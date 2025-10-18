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
#include <math.h>

// Estrutura para armazenar fatores primos e seus expoentes
typedef struct {
    int primo;
    int expoente;
} FatorPrimo;

// Funcao para verificar se um numero e primo
int eh_primo(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Funcao para fatoracao prima usando Trial Division
int fatoracao_prima(int n, FatorPrimo fatores[]) {
    printf("\n=== FATORACAO PRIMA DE %d ===\n", n);
    
    int num_fatores = 0;
    int n_original = n;
    
    // Verificar divisibilidade por 2
    if (n % 2 == 0) {
        int expoente = 0;
        while (n % 2 == 0) {
            n /= 2;
            expoente++;
        }
        fatores[num_fatores].primo = 2;
        fatores[num_fatores].expoente = expoente;
        printf("Fator encontrado: %d^%d\n", 2, expoente);
        num_fatores++;
    }
    
    // Verificar divisibilidade por numeros impares
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            int expoente = 0;
            while (n % i == 0) {
                n /= i;
                expoente++;
            }
            fatores[num_fatores].primo = i;
            fatores[num_fatores].expoente = expoente;
            printf("Fator encontrado: %d^%d\n", i, expoente);
            num_fatores++;
        }
    }
    
    // Se sobrou algum fator primo maior que 1
    if (n > 1) {
        fatores[num_fatores].primo = n;
        fatores[num_fatores].expoente = 1;
        printf("Fator encontrado: %d^1\n", n);
        num_fatores++;
    }
    
    printf("Fatoracao completa: ");
    for (int i = 0; i < num_fatores; i++) {
        printf("%d^%d", fatores[i].primo, fatores[i].expoente);
        if (i < num_fatores - 1) printf(" x ");
    }
    printf(" = %d\n", n_original);
    
    return num_fatores;
}

// Funcao para calcular tau(N) - numero de divisores
int calcular_tau(FatorPrimo fatores[], int num_fatores) {
    printf("\n=== CALCULO DE TAU(N) ===\n");
    printf("Formula: tau(N) = (e1 + 1)(e2 + 1)...(ek + 1)\n");
    
    int tau = 1;
    printf("Calculando: ");
    
    for (int i = 0; i < num_fatores; i++) {
        int termo = fatores[i].expoente + 1;
        tau *= termo;
        printf("(%d + 1)", fatores[i].expoente);
        if (i < num_fatores - 1) printf(" x ");
        else printf(" = ");
    }
    
    printf("%d", tau);
    for (int i = 0; i < num_fatores; i++) {
        int termo = fatores[i].expoente + 1;
        printf(" = %d", termo);
        if (i < num_fatores - 1) printf(" x ");
    }
    printf(" = %d\n", tau);
    
    printf("tau(%d) = %d\n", fatores[0].primo, tau);
    return tau;
}

// Funcao para calcular sigma(N) - soma dos divisores
double calcular_sigma(FatorPrimo fatores[], int num_fatores) {
    printf("\n=== CALCULO DE SIGMA(N) ===\n");
    printf("Formula: sigma(N) = [(p1^(e1+1) - 1)/(p1 - 1)] x ... x [(pk^(ek+1) - 1)/(pk - 1)]\n");
    
    double sigma = 1.0;
    printf("Calculando: ");
    
    for (int i = 0; i < num_fatores; i++) {
        int p = fatores[i].primo;
        int e = fatores[i].expoente;
        
        // Calcular p^(e+1)
        double p_elevado = pow(p, e + 1);
        double termo = (p_elevado - 1) / (p - 1);
        
        sigma *= termo;
        
        printf("[%d^(%d+1) - 1]/[%d - 1]", p, e, p);
        if (i < num_fatores - 1) printf(" x ");
        else printf(" = ");
    }
    
    // Mostrar calculos detalhados
    printf("\nDetalhamento dos calculos:\n");
    for (int i = 0; i < num_fatores; i++) {
        int p = fatores[i].primo;
        int e = fatores[i].expoente;
        
        double p_elevado = pow(p, e + 1);
        double termo = (p_elevado - 1) / (p - 1);
        
        printf("  [%d^(%d+1) - 1]/[%d - 1] = [%d^%d - 1]/%d = [%.0f - 1]/%d = %.0f/%d = %.0f\n",
               p, e, p, p, e+1, p-1, p_elevado, p-1, p_elevado-1, p-1, termo);
    }
    
    printf("sigma = ");
    for (int i = 0; i < num_fatores; i++) {
        int p = fatores[i].primo;
        int e = fatores[i].expoente;
        double p_elevado = pow(p, e + 1);
        double termo = (p_elevado - 1) / (p - 1);
        printf("%.0f", termo);
        if (i < num_fatores - 1) printf(" x ");
    }
    printf(" = %.0f\n", sigma);
    
    return sigma;
}

// Funcao para calcular a razao de eficiencia
double calcular_razao_eficiencia(double sigma, int tau) {
    printf("\n=== CALCULO DA RAZAO DE EFICIENCIA ===\n");
    printf("Formula: Razao = sigma(N) / tau(N)\n");
    printf("Razao = %.0f / %d = %.2f\n", sigma, tau, sigma / tau);
    
    return sigma / tau;
}

// Funcao para exibir resumo dos resultados
void exibir_resumo(int n, FatorPrimo fatores[], int num_fatores, int tau, double sigma, double razao) {
    printf("\n=== RESUMO DOS RESULTADOS ===\n");
    printf("Numero analisado: %d\n", n);
    
    printf("Fatoracao prima: ");
    for (int i = 0; i < num_fatores; i++) {
        printf("%d^%d", fatores[i].primo, fatores[i].expoente);
        if (i < num_fatores - 1) printf(" x ");
    }
    printf("\n");
    
    printf("tau(%d) = %d (numero de divisores)\n", n, tau);
    printf("sigma(%d) = %.0f (soma dos divisores)\n", n, sigma);
    printf("Razao de Eficiencia = %.2f\n", razao);
}

int main() {
    printf("=== CALCULADORA DE RAZAO DE EFICIENCIA ===\n");
    printf("Autor: Thiago Melo Tonin\n");
    printf("Matricula: 221022453\n\n");
    
    int n;
    FatorPrimo fatores[20]; // Array para armazenar fatores primos
    int num_fatores;
    
    // Entrada de dados
    printf("Digite um numero inteiro N (1 <= N <= 100000): ");
    scanf("%d", &n);
    
    // Validacao da entrada
    if (n < 1 || n > 100000) {
        printf("Erro: Numero deve estar entre 1 e 100000.\n");
        return 1;
    }
    
    printf("\nAnalisando o numero %d...\n", n);
    
    // Casos especiais
    if (n == 1) {
        printf("\n=== CASO ESPECIAL: N = 1 ===\n");
        printf("Fatoracao prima: 1 (sem fatores primos)\n");
        printf("tau(1) = 1 (apenas o proprio 1)\n");
        printf("sigma(1) = 1 (soma dos divisores)\n");
        printf("Razao de Eficiencia = 1.00\n");
        return 0;
    }
    
    // Etapa 1: Fatoracao prima
    num_fatores = fatoracao_prima(n, fatores);
    
    // Etapa 2: Calcular tau(N)
    int tau = calcular_tau(fatores, num_fatores);
    
    // Etapa 3: Calcular sigma(N)
    double sigma = calcular_sigma(fatores, num_fatores);
    
    // Etapa 4: Calcular razao de eficiencia
    double razao = calcular_razao_eficiencia(sigma, tau);
    
    // Exibir resumo final
    exibir_resumo(n, fatores, num_fatores, tau, sigma, razao);
    
    printf("\n=== PROGRAMA CONCLUIDO ===\n");
    return 0;
}
