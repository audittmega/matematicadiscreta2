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

// Funcao para calcular MDC usando Algoritmo de Euclides
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

// Algoritmo de Euclides Estendido (para encontrar inverso modular)
long long euclides_estendido(long long a, long long b, long long *x, long long *y) {
    printf("\n=== ALGORITMO DE EUCLIDES ESTENDIDO ===\n");
    printf("Calculando inverso modular de %lld mod %lld\n", a, b);
    
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    
    long long x1, y1;
    long long gcd = euclides_estendido(b % a, a, &x1, &y1);
    
    *x = y1 - (b / a) * x1;
    *y = x1;
    
    printf("Coeficientes: x = %lld, y = %lld\n", *x, *y);
    return gcd;
}

// Funcao para encontrar inverso modular
long long inverso_modular(long long a, long long modulo) {
    printf("\n=== CALCULO DO INVERSO MODULAR ===\n");
    
    long long x, y;
    long long gcd = euclides_estendido(a, modulo, &x, &y);
    
    if (gcd != 1) {
        printf("Erro: MDC(%lld, %lld) = %lld diferente de 1\n", a, modulo, gcd);
        printf("Nao existe inverso modular!\n");
        return -1;
    }
    
    // Ajustar valor negativo somando o modulo
    long long inverso = (x % modulo + modulo) % modulo;
    
    printf("Inverso modular de %lld mod %lld = %lld\n", a, modulo, inverso);
    printf("Verificacao: (%lld x %lld) mod %lld = %lld\n", 
           a, inverso, modulo, (a * inverso) % modulo);
    
    return inverso;
}

// Funcao para verificar se um numero e primo
int eh_primo(long long n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    
    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Funcao para calcular phi(n) - funcao totiente de Euler
long long calcular_phi(long long n) {
    long long resultado = n;
    
    for (long long p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) {
                n /= p;
            }
            resultado -= resultado / p;
        }
    }
    
    if (n > 1) {
        resultado -= resultado / n;
    }
    
    return resultado;
}

// Funcao para exponenciacao modular com selecao automatica do teorema
long long exponenciacao_modular(long long base, long long expoente, long long modulo) {
    printf("\n=== EXPONENCIACAO MODULAR ===\n");
    printf("Calculando %lld^%lld mod %lld\n", base, expoente, modulo);
    
    // Verificar se o modulo e primo
    int modulo_primo = eh_primo(modulo);
    printf("Modulo %lld e primo? %s\n", modulo, modulo_primo ? "Sim" : "Nao");
    
    // Verificar MDC(base, modulo)
    long long gcd_base_mod = mdc_euclides(base, modulo);
    printf("MDC(%lld, %lld) = %lld\n", base, modulo, gcd_base_mod);
    
    // Selecionar teorema automaticamente
    if (modulo_primo && gcd_base_mod == 1) {
        printf("\n=== APLICANDO PEQUENO TEOREMA DE FERMAT ===\n");
        printf("Como %lld e primo e MDC(%lld, %lld) = 1\n", modulo, base, modulo);
        printf("Temos: %lld^%lld = %lld^(%lld-1) = 1 (mod %lld)\n", 
               base, modulo-1, base, modulo, modulo);
        
        // Reduzir expoente usando Fermat
        expoente = expoente % (modulo - 1);
        printf("Expoente reduzido: %lld mod %lld = %lld\n", 
               expoente + (modulo - 1), modulo - 1, expoente);
        
    } else if (gcd_base_mod == 1) {
        printf("\n=== APLICANDO TEOREMA DE EULER ===\n");
        long long phi = calcular_phi(modulo);
        printf("phi(%lld) = %lld\n", modulo, phi);
        printf("Como MDC(%lld, %lld) = 1, temos: %lld^%lld = 1 (mod %lld)\n", 
               base, modulo, base, phi, modulo);
        
        // Reduzir expoente usando Euler
        expoente = expoente % phi;
        printf("Expoente reduzido: %lld mod %lld = %lld\n", 
               expoente + phi, phi, expoente);
        
    } else {
        printf("\n=== USANDO EXPONENCIACAO BINARIA ===\n");
        printf("MDC(%lld, %lld) = %lld diferente de 1, usando exponenciacao binaria\n", 
               base, modulo, gcd_base_mod);
    }
    
    // Exponenciacao binaria
    printf("\nPassos da exponenciacao binaria:\n");
    long long resultado = 1;
    base = base % modulo;
    
    int passo = 0;
    while (expoente > 0) {
        passo++;
        if (expoente % 2 == 1) {
            long long resultado_anterior = resultado;
            resultado = (resultado * base) % modulo;
            printf("Passo %d: Expoente impar, resultado = (%lld x %lld) mod %lld = %lld\n", 
                   passo, resultado_anterior, base, modulo, resultado);
        }
        
        expoente = expoente >> 1;
        if (expoente > 0) {
            long long base_anterior = base;
            base = (base * base) % modulo;
            printf("Passo %d: Expoente = %lld, base = %lld^2 mod %lld = %lld\n", 
                   passo, expoente, base_anterior, modulo, base);
        }
    }
    
    printf("Resultado final: %lld^%lld mod %lld = %lld\n", 
           base, expoente, modulo, resultado);
    
    return resultado;
}

// Funcao para divisao modular H / G (mod Zn)
long long divisao_modular(long long H, long long G, long long Zn) {
    printf("\n=== DIVISAO MODULAR ===\n");
    printf("Calculando (%lld / %lld) mod %lld\n", H, G, Zn);
    
    // Verificar se G tem inverso modular em Zn
    long long mdc = mdc_euclides(G, Zn);
    if (mdc != 1) {
        printf("Erro: MDC(%lld, %lld) = %lld diferente de 1\n", G, Zn, mdc);
        printf("Nao e possivel fazer divisao modular!\n");
        return -1;
    }
    
    // Calcular inverso modular de G
    long long inverso_G = inverso_modular(G, Zn);
    if (inverso_G == -1) {
        return -1;
    }
    
    // Calcular divisao modular: H / G = H * G^(-1) (mod Zn)
    long long resultado = (H * inverso_G) % Zn;
    
    printf("Divisao modular: (%lld / %lld) mod %lld = (%lld x %lld) mod %lld = %lld\n",
           H, G, Zn, H, inverso_G, Zn, resultado);
    
    return resultado;
}

int main() {
    printf("=== CALCULADORA MODULAR AVANCADA ===\n");
    printf("Autor: Thiago Melo Tonin\n");
    printf("Matricula: 221022453\n\n");
    
    // Parametros de teste fornecidos
    long long H = 7, G = 3, Zn = 11, x = 10, n1 = 13;
    
    printf("=== PARAMETROS DE TESTE ===\n");
    printf("H = %lld\n", H);
    printf("G = %lld\n", G);
    printf("Zn = %lld\n", Zn);
    printf("x = %lld\n", x);
    printf("n1 = %lld\n", n1);
    
    // Tarefa 1: Divisao modular H / G (mod Zn)
    printf("\n========================================\n");
    printf("TAREFA 1: DIVISAO MODULAR H / G (mod Zn)\n");
    printf("========================================\n");
    
    long long resultado_divisao = divisao_modular(H, G, Zn);
    
    // Tarefa 2: Exponenciacao modular a^x mod n1
    printf("\n========================================\n");
    printf("TAREFA 2: EXPONENCIACAO MODULAR a^x mod n1\n");
    printf("========================================\n");
    
    // Usar H como base para a^x mod n1
    long long resultado_exponenciacao = exponenciacao_modular(H, x, n1);
    
    // Resultados finais
    printf("\n========================================\n");
    printf("RESULTADOS FINAIS\n");
    printf("========================================\n");
    
    printf("1. Divisao modular (%lld / %lld) mod %lld = %lld\n", 
           H, G, Zn, resultado_divisao);
    
    printf("2. Exponenciacao modular %lld^%lld mod %lld = %lld\n", 
           H, x, n1, resultado_exponenciacao);
    
    // Analise teorica
    printf("\n========================================\n");
    printf("ANALISE TEORICA\n");
    printf("========================================\n");
    
    printf("Afirmativas sobre o algoritmo:\n\n");
    
    printf("1. (V) O Algoritmo de Euclides Estendido permite calcular o inverso modular\n");
    printf("   Justificativa: Verdadeiro. O algoritmo encontra x tal que ax ≡ 1 (mod n)\n\n");
    
    printf("2. (V) A divisao modular H/G mod n e equivalente a H * G^(-1) mod n\n");
    printf("   Justificativa: Verdadeiro. Em aritmetica modular, divisao e multiplicacao pelo inverso\n\n");
    
    printf("3. (V) O Pequeno Teorema de Fermat so se aplica quando o modulo e primo\n");
    printf("   Justificativa: Verdadeiro. Fermat: a^(p-1) ≡ 1 (mod p) para p primo\n\n");
    
    printf("4. (V) O Teorema de Euler se aplica quando MDC(a,n) = 1\n");
    printf("   Justificativa: Verdadeiro. Euler: a^φ(n) ≡ 1 (mod n) quando MDC(a,n) = 1\n\n");
    
    printf("5. (F) Todo numero tem inverso modular em qualquer modulo\n");
    printf("   Justificativa: Falso. Apenas quando MDC(numero, modulo) = 1\n\n");
    
    printf("=== PROGRAMA CONCLUIDO ===\n");
    
    return 0;
}
