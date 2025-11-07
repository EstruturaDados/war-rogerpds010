#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura que representa um território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* Cadastra territórios em um vetor alocado dinamicamente.
 * Uso de ponteiros para acessar e modificar os dados.
 */
void cadastrarTerritorios(Territorio *mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("\n>>> Cadastro do Território %d <<<\n", i + 1);

        printf("Digite o nome do território: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0'; // remove o \n

        printf("Digite a cor do exército: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0'; // remove o \n

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        while (getchar() != '\n'); // limpa o buffer
    }
}

/* Exibe todos os territórios (acesso via ponteiro) */
void exibirTerritorios(Territorio *mapa, int qtd) {
    printf("\n=====================================\n");
    printf("        DADOS DOS TERRITÓRIOS\n");
    printf("=====================================\n");
    for (int i = 0; i < qtd; i++) {
        printf("\n[%d] Nome: %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("\n");
}

/* Simula um ataque entre dois territórios.
 * Regras:
 * - Se atacar um território da mesma cor -> inválido.
 * - Atacante precisa ter > 1 tropa para atacar.
 * - Cada lado rola um dado (1-6).
 * - Se atacante vence: defensor muda de cor e recebe metade das tropas do atacante.
 *   (essas tropas são transferidas do atacante, ou seja, atacante perde essa metade)
 * - Se atacante perde: atacante perde 1 tropa.
 */
void atacar(Territorio *atacante, Territorio *defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[ERRO] Você não pode atacar um território da mesma cor!\n");
        return;
    }

    if (atacante->tropas <= 1) {
        printf("\n[ERRO] O território atacante precisa ter mais de 1 tropa para atacar!\n");
        return;
    }

    printf("\n--- Simulando ataque entre '%s' (Cor: %s, Tropas: %d) "
           "e '%s' (Cor: %s, Tropas: %d) ---\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor  = rand() % 6 + 1;

    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor:  %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\nO atacante venceu a batalha!\n");
        int transfer = atacante->tropas / 2; // metade das tropas do atacante
        if (transfer <= 0) transfer = 1; // garantia mínima (caso atacante tenha 1 mas já checado)
        // transfere tropas e cor
        atacante->tropas -= transfer;
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = transfer;
        printf("'%s' agora pertence ao exército %s e recebeu %d tropas.\n",
               defensor->nome, defensor->cor, defensor->tropas);
        printf("O atacante '%s' ficou com %d tropas.\n", atacante->nome, atacante->tropas);
    } else {
        printf("\nO defensor resistiu ao ataque!\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("O atacante '%s' perdeu 1 tropa (agora tem %d).\n",
               atacante->nome, atacante->tropas);
    }
}

/* Libera a memória alocada dinamicamente para o mapa */
void liberarMemoria(Territorio *mapa) {
    free(mapa);
    printf("\nMemória liberada com sucesso!\n");
}

// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    srand((unsigned int)time(NULL)); // inicializa gerador de números aleatórios

    int qtd;
    printf("Informe o número de territórios: ");
    if (scanf("%d", &qtd) != 1 || qtd <= 0) {
        printf("Quantidade inválida. Encerrando.\n");
        return 1;
    }
    while (getchar() != '\n'); // limpa buffer

    // Alocação dinâmica de territórios
    Territorio *mapa = (Territorio *)calloc(qtd, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    // Cadastro inicial
    cadastrarTerritorios(mapa, qtd);

    // Loop de turnos: continua executando ataques **um por vez** até o usuário digitar 0 para sair
    while (1) {
        exibirTerritorios(mapa, qtd);

        printf("Digite o número do território atacante (1-%d) ou 0 para sair: ", qtd);
        int iAtacante;
        if (scanf("%d", &iAtacante) != 1) {
            printf("Entrada inválida. Encerrando.\n");
            break;
        }
        while (getchar() != '\n'); // limpa buffer

        if (iAtacante == 0) {
            printf("\nVocê escolheu encerrar o jogo. Saindo...\n");
            break; // sai do loop e finaliza
        }
        if (iAtacante < 1 || iAtacante > qtd) {
            printf("\n[ERRO] Território atacante inválido. Tente novamente.\n");
            continue;
        }

        printf("Digite o número do território defensor (1-%d): ", qtd);
        int iDefensor;
        if (scanf("%d", &iDefensor) != 1) {
            printf("Entrada inválida. Encerrando.\n");
            break;
        }
        while (getchar() != '\n'); // limpa buffer

        if (iDefensor < 1 || iDefensor > qtd) {
            printf("\n[ERRO] Território defensor inválido. Tente novamente.\n");
            continue;
        }

        if (iAtacante == iDefensor) {
            printf("\n[ERRO] Um território não pode atacar a si mesmo. Tente novamente.\n");
            continue;
        }

        // Realiza ataque usando ponteiros para os territórios selecionados
        atacar(&mapa[iAtacante - 1], &mapa[iDefensor - 1]);

        // Após o ataque, exibir resumo breve do resultado (o loop exibirá a lista completa no início do próximo turno)
        printf("\n--- Estado após o ataque ---\n");
        printf("[%d] %s | Cor: %s | Tropas: %d\n",
               iAtacante, mapa[iAtacante - 1].nome, mapa[iAtacante - 1].cor, mapa[iAtacante - 1].tropas);
        printf("[%d] %s | Cor: %s | Tropas: %d\n\n",
               iDefensor, mapa[iDefensor - 1].nome, mapa[iDefensor - 1].cor, mapa[iDefensor - 1].tropas);
    }

    // Libera memória antes de encerrar
    liberarMemoria(mapa);

    printf("Programa finalizado.\n");
    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
