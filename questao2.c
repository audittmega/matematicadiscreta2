/*
---------------------------------
Autor: Thiago Melo Tonin
Matricula: 221022453
Disciplina: Matematica Discreta II
Professor: Cristiane Loesch
---------------------------------
*/

#include <stdio.h>
#include <stdlib.h>

// Funcao para calcular o MDC usando Algoritmo de Euclides
long long mdc_euclides(long long a, long long b) {
    printf("  Calculando MDC(%lld, %lld):\n", a, b);
    
    long long original_a = a, original_b = b;
    int passo = 0;
    
    while (b != 0) {
        passo++;
        long long resto = a % b;
        printf("    Passo %d: %lld = %lld x %lld + %lld\n", 
               passo, a, b, a/b, resto);
        a = b;
        b = resto;
    }
    
    printf("    Resultado: MDC(%lld, %lld) = %lld\n", original_a, original_b, a);
    return a;
}

// Funcao para calcular o MMC usando a relacao MMC(a,b) = (a x b) / MDC(a,b)
long long mmc_dois_numeros(long long a, long long b) {
    printf("\nCalculando MMC(%lld, %lld):\n", a, b);
    
    long long mdc = mdc_euclides(a, b);
    long long mmc = (a * b) / mdc;
    
    printf("  MMC(%lld, %lld) = (%lld x %lld) / %lld = %lld\n", 
           a, b, a, b, mdc, mmc);
    
    return mmc;
}

// Funcao para calcular o MMC de multiplos numeros
long long mmc_multiplos_numeros(int ciclos[], int n) {
    printf("\n=== CALCULO DO MMC DE MULTIPLOS NUMEROS ===\n");
    
    if (n == 1) {
        printf("Apenas um ciclo: MMC = %d\n", ciclos[0]);
        return ciclos[0];
    }
    
    // Comecar com o primeiro numero
    long long resultado = ciclos[0];
    printf("Iniciando com o primeiro ciclo: %lld\n", resultado);
    
    // Calcular MMC progressivamente com cada novo ciclo
    for (int i = 1; i < n; i++) {
        printf("\n--- Passo %d: Incluindo ciclo %d (%d) ---\n", i, i+1, ciclos[i]);
        resultado = mmc_dois_numeros(resultado, ciclos[i]);
        printf("MMC parcial apos %d ciclos: %lld\n", i+1, resultado);
    }
    
    return resultado;
}

// Funcao para verificar se um ano e valido para sincronizacao
void verificar_sincronizacao(int ciclos[], int n, long long mmc) {
    printf("\n=== VERIFICACAO DE SINCRONIZACAO ===\n");
    
    if (mmc <= 50) {
        printf("Sincronizacao possivel!\n");
        printf("Primeiro ano de sincronizacao: %lld\n", mmc);
        
        printf("\nVerificacao:\n");
        for (int i = 0; i < n; i++) {
            printf("Chave %d (ciclo %d): %lld / %d = %lld (resto: %lld)\n", 
                   i+1, ciclos[i], mmc, ciclos[i], mmc/ciclos[i], mmc%ciclos[i]);
        }
    } else {
        printf("Sincronizacao nao e possivel dentro do limite de 50 anos.\n");
        printf("MMC calculado: %lld > 50\n", mmc);
    }
}

// Funcao para exibir informacoes sobre os ciclos
void exibir_ciclos(int ciclos[], int n) {
    printf("\n=== INFORMACOES DOS CICLOS ===\n");
    printf("Numero de chaves: %d\n", n);
    printf("Ciclos das chaves: ");
    for (int i = 0; i < n; i++) {
        printf("%d", ciclos[i]);
        if (i < n-1) printf(", ");
    }
    printf("\n");
    
    printf("\nExplicacao dos ciclos:\n");
    for (int i = 0; i < n; i++) {
        printf("Chave %d: ativa a cada %d anos\n", i+1, ciclos[i]);
    }
}

int main() {
    printf("=== SINCRONIZACAO DE CHAVES PERIODICAS ===\n");
    printf("Autor: Thiago Melo Tonin\n");
    printf("Matricula: 221022453\n\n");
    
    int n;
    int ciclos[10];
    
    // Entrada de dados
    printf("Digite o numero de chaves (1 <= N <= 10): ");
    scanf("%d", &n);
    
    // Validacao do numero de chaves
    if (n < 1 || n > 10) {
        printf("Erro: Numero de chaves deve estar entre 1 e 10.\n");
        return 1;
    }
    
    printf("Digite os ciclos das %d chaves (2 <= Ci <= 20):\n", n);
    for (int i = 0; i < n; i++) {
        printf("Ciclo da chave %d: ", i+1);
        scanf("%d", &ciclos[i]);
        
        // Validacao dos ciclos
        if (ciclos[i] < 2 || ciclos[i] > 20) {
            printf("Erro: Ciclo deve estar entre 2 e 20.\n");
            return 1;
        }
    }
    
    // Exibir informacoes dos ciclos
    exibir_ciclos(ciclos, n);
    
    // Calcular MMC de todos os ciclos
    long long mmc_resultado = mmc_multiplos_numeros(ciclos, n);
    
    // Verificar sincronizacao
    verificar_sincronizacao(ciclos, n, mmc_resultado);
    
    // Resultado final
    printf("\n=== RESULTADO FINAL ===\n");
    if (mmc_resultado <= 50) {
        printf("O primeiro ano em que todas as chaves podem ser usadas simultaneamente e: %lld\n", mmc_resultado);
    } else {
        printf("Nao e possivel sincronizar as chaves dentro do limite de 50 anos.\n");
    }
    
    printf("\n=== EXPLICACAO DO METODO ===\n");
    printf("O problema consiste em encontrar o menor multiplo comum (MMC) de todos os ciclos.\n");
    printf("O MMC representa o menor numero que e multiplo de todos os ciclos simultaneamente.\n");
    printf("Formula: MMC(a,b) = (a x b) / MDC(a,b)\n");
    printf("Para multiplos numeros: MMC(a,b,c,...) = MMC(MMC(a,b), c, ...)\n");
    
    return 0;
}
