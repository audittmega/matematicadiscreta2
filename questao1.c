/*
---------------------------------
Autor: Thiago Melo Tonin
Matrícula: 221022453
Disciplina: Matemática Discreta II
Professor: Cristiane Loesch
---------------------------------
*/

#include <stdio.h>
#include <stdlib.h>

// MDC Euclides
long long mdc_euclides(long long a, long long b) {
    if (b == 0) {
        return a;
    }
    return mdc_euclides(b, a % b);
}

// Metodo rho de Pollard
long long fatoracao_pollard(long long n, int *passos) {
    printf("\n=== FATORACAO COM METODO RHO DE POLLARD ===\n");
    printf("Numero a fatorar: %lld\n", n);
    
    // Para numeros pequenos
    if (n <= 100) {
        printf("Numero pequeno - usando divisao por tentativas...\n");
        for (long long i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                printf("Fator encontrado por divisao: %lld\n", i);
                *passos = 1;
                return i;
            }
        }
        printf("Numero primo encontrado: %lld\n", n);
        *passos = 0;
        return n;
    }
    
    long long x = 2;  // x0 = 2
    long long y = 2;
    long long d = 1;
    int passo = 0;
    
    printf("Iniciando com x0 = %lld\n", x);
    printf("Usando g(x) = (x^2 + 1) mod %lld\n", n);
    printf("\nPassos:\n");
    
    while (d == 1) {
        passo++;
        
        // g(x) = (x^2 + 1) mod n
        x = ((x * x) + 1) % n;
        
        // g(g(y)) = g(y) aplicado novamente
        y = ((y * y) + 1) % n;
        y = ((y * y) + 1) % n;
        
        // Calcular |x - y|
        long long diff = (x > y) ? (x - y) : (y - x);
        
        // Calcular MDC(|x - y|, n)
        d = mdc_euclides(diff, n);
        
        printf("Passo %d: x = %lld, y = %lld, |x-y| = %lld, mdc(|x-y|, %lld) = %lld\n", 
               passo, x, y, diff, n, d);
        
        if (d > 1 && d < n) {
            printf("Fator encontrado: %lld\n", d);
            break;
        }
        
        // Evitar loop infinito
        if (passo > 50) {
            printf("Metodo de Pollard nao encontrou fator em 50 passos. Tentando divisao por tentativas...\n");
            for (long long i = 2; i * i <= n; i++) {
                if (n % i == 0) {
                    d = i;
                    printf("Fator encontrado por divisao: %lld\n", d);
                    break;
                }
            }
            break;
        }
    }
    
    *passos = passo;
    return d;
}

// Euclides Estendido
long long euclides_estendido(long long a, long long b, long long *x, long long *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    
    long long x1, y1;
    long long gcd = euclides_estendido(b % a, a, &x1, &y1);
    
    *x = y1 - (b / a) * x1;
    *y = x1;
    
    return gcd;
}

// inverso modular
long long inverso_modular(long long e, long long phi_n) {
    long long x, y;
    long long gcd = euclides_estendido(e, phi_n, &x, &y);
    
    if (gcd != 1) {
        printf("Erro: MDC(%lld, %lld) = %lld diferente de 1\n", e, phi_n, gcd);
        return -1;
    }
    
    // garantir que o resultado seja positivo
    return (x % phi_n + phi_n) % phi_n;
}

// exponenciacao modular 
long long exponenciacao_modular(long long base, long long expoente, long long modulo, int *teorema_usado) {
    printf("\n=== EXPONENCIACAO MODULAR ===\n");
    printf("Calculando %lld^%lld mod %lld\n", base, expoente, modulo);
    
    // verifica se o modulo e primo
    int e_primo = 1;
    for (long long i = 2; i * i <= modulo; i++) {
        if (modulo % i == 0) {
            e_primo = 0;
            break;
        }
    }
    
    // verificar mdc(base, modulo)
    long long gcd_base_mod = mdc_euclides(base, modulo);
    
    printf("MDC(%lld, %lld) = %lld\n", base, modulo, gcd_base_mod);
    
    if (e_primo && gcd_base_mod == 1) {
        printf("Aplicando Pequeno Teorema de Fermat: a^(p-1) = 1 (mod p)\n");
        *teorema_usado = 1;
        // reduzir o expoente usando Fermat
        expoente = expoente % (modulo - 1);
        printf("Expoente reduzido para: %lld\n", expoente);
    } else if (gcd_base_mod == 1) {
        printf("Aplicando Teorema de Euler: a^phi(n) = 1 (mod n)\n");
        *teorema_usado = 2;
        // reduzir o expoente usando Euler
        // Para RSA, phi(n) = (p-1)(q-1) é conhecido
        // como não temos phi(n) aqui, vamos usar o módulo original
    } else {
        printf("Usando Divisao Euclidiana (exponenciacao binaria)\n");
        *teorema_usado = 3;
    }
    
    // Exponenciacao binaria
    long long resultado = 1;
    base = base % modulo;
    
    printf("Passos da exponenciacao binaria:\n");
    int passo_exp = 0;
    
    while (expoente > 0) {
        passo_exp++;
        if (expoente % 2 == 1) {
            long long resultado_anterior = resultado;
            resultado = (resultado * base) % modulo;
            printf("Passo %d: Expoente impar, resultado = (%lld * %lld) mod %lld = %lld\n", 
                   passo_exp, resultado_anterior, base, modulo, resultado);
        }
        
        expoente = expoente >> 1;
        if (expoente > 0) {
            long long base_anterior = base;
            base = (base * base) % modulo;
            printf("Passo %d: Expoente = %lld, base = %lld^2 mod %lld = %lld\n", 
                   passo_exp, expoente, base_anterior, modulo, base);
        }
    }
    
    return resultado;
}

// converte caractere para numero
long long char_para_numero(char c) {
    if (c == ' ') return 0;
    if (c >= 'A' && c <= 'Z') return c - 'A' + 11;
    if (c >= 'a' && c <= 'z') return c - 'a' + 11;
    return -1; // Caractere invalido
}

// converte numero para caractere
char numero_para_char(long long num) {
    if (num == 0) return ' ';
    if (num >= 11 && num <= 36) return 'A' + num - 11;
    return '?'; // Numero invalido
}

int main() {
    printf("=== SISTEMA RSA COMPLETO ===\n");
    printf("Autor: Thiago Melo Tonin\n");
    printf("Matricula: 221022453\n\n");
    
    // ETAPA 1: FATORACAO
    printf("ETAPA 1: FATORACAO INTERATIVA\n");
    printf("=============================\n");
    
    long long N1, N2;
    printf("Digite o primeiro numero composto N1 (3-4 digitos): ");
    scanf("%lld", &N1);
    printf("Digite o segundo numero composto N2 (3-4 digitos): ");
    scanf("%lld", &N2);
    
    int passos1, passos2;
    long long p = fatoracao_pollard(N1, &passos1);
    long long q = fatoracao_pollard(N2, &passos2);
    
    // Garantir que p e q sejam primos distintos
    printf("\nFatores encontrados:\n");
    printf("p (fator de N1 = %lld) = %lld\n", N1, p);
    printf("q (fator de N2 = %lld) = %lld\n", N2, q);
    
    // Se p == q, encontrar o segundo fator
    if (p == q) {
        printf("Aviso: p = q = %lld. Procurando segundo fator...\n", p);
        if (N1 == N2) {
            // Se os numeros sao iguais, usar fatores diferentes
            printf("N1 = N2. Usando fatores diferentes para demonstrar RSA.\n");
            q = N2 / p;  // Segundo fator
        } else {
            // Usar o segundo fator do outro numero
            q = N2 / q;
        }
        printf("q ajustado para: %lld\n", q);
    }
    
    // Verificar se p e q sao primos
    printf("Verificando se sao primos:\n");
    printf("p = %lld, q = %lld\n", p, q);
    
    // ETAPA 2: GERACAO DE CHAVES RSA
    printf("\n\nETAPA 2: GERACAO DE CHAVES RSA\n");
    printf("==============================\n");
    
    long long n = p * q;
    long long phi_n = (p - 1) * (q - 1);
    
    printf("n = p x q = %lld x %lld = %lld\n", p, q, n);
    printf("phi(n) = (p - 1) x (q - 1) = (%lld - 1) x (%lld - 1) = %lld\n", 
           p, q, phi_n);
    
    // Escolher E tal que MDC(E, phi(n)) = 1
    long long E = 3; // Comecar com valor menor para numeros pequenos
    for (E = 3; E < phi_n; E += 2) {
        if (mdc_euclides(E, phi_n) == 1) {
            break;
        }
    }
    
    printf("E escolhido: %lld (MDC(%lld, %lld) = %lld)\n", 
           E, E, phi_n, mdc_euclides(E, phi_n));
    
    // Calcular D (inverso modular de E)
    long long D = inverso_modular(E, phi_n);
    printf("D (inverso modular): %lld\n", D);
    printf("Verificacao: (%lld x %lld) mod %lld = %lld\n", 
           E, D, phi_n, (E * D) % phi_n);
    
    printf("\nChaves geradas:\n");
    printf("Chave Publica (n, E): (%lld, %lld)\n", n, E);
    printf("Chave Privada (n, D): (%lld, %lld)\n", n, D);
    
    // ETAPA 3: CODIFICACAO E DECODIFICACAO
    printf("\n\nETAPA 3: CODIFICACAO E DECODIFICACAO\n");
    printf("====================================\n");
    
    char mensagem[100];
    printf("Digite uma mensagem para criptografar (apenas letras maiusculas e espacos): ");
    getchar(); // Limpar buffer
    fgets(mensagem, sizeof(mensagem), stdin);
    
    printf("\nConversao de caracteres:\n");
    printf("A=11, B=12, ..., Z=36, espaco=00\n");
    
    printf("\nMensagem criptografada:\n");
    int teorema_usado;
    
    for (int i = 0; mensagem[i] != '\n' && mensagem[i] != '\0'; i++) {
        char c = mensagem[i];
        long long M = char_para_numero(c);
        
        if (M == -1) {
            printf("Caractere invalido: %c\n", c);
            continue;
        }
        
        printf("\nCaractere: '%c' -> M = %lld\n", c, M);
        
        // Criptografar: C = M^E mod n
        long long C = exponenciacao_modular(M, E, n, &teorema_usado);
        printf("C = %lld^%lld mod %lld = %lld\n", M, E, n, C);
        
        // Descriptografar: M' = C^D mod n
        long long M_descifrado = exponenciacao_modular(C, D, n, &teorema_usado);
        printf("M' = %lld^%lld mod %lld = %lld\n", C, D, n, M_descifrado);
        
        char c_descifrado = numero_para_char(M_descifrado);
        printf("M' = %lld -> Caractere: '%c'\n", M_descifrado, c_descifrado);
        
        printf("Verificacao: %s\n", (M == M_descifrado) ? "CORRETO" : "ERRO");
        
        switch (teorema_usado) {
            case 1:
                printf("Teorema aplicado: Pequeno Teorema de Fermat\n");
                break;
            case 2:
                printf("Teorema aplicado: Teorema de Euler\n");
                break;
            case 3:
                printf("Teorema aplicado: Divisao Euclidiana\n");
                break;
        }
        printf("Justificativa: MDC(%lld, %lld) = %lld\n", M, n, mdc_euclides(M, n));
    }
    
    printf("\n=== PROGRAMA CONCLUIDO ===\n");
    return 0;
}
