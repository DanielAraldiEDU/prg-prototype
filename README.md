# Protótipo de RPG

[![wakatime](https://wakatime.com/badge/user/920a7e43-2969-4212-82ff-1b375685ff58/project/7be698d6-5baf-4d54-8205-701a376cd9e3.svg)](https://wakatime.com/badge/user/920a7e43-2969-4212-82ff-1b375685ff58/project/7be698d6-5baf-4d54-8205-701a376cd9e3)

Protótipo de RPG de ponteiros e alocação dinâmica feito em C++ por [Daniel Sansão Araldi](https://github.com/DanielAraldi), [Mateus Vanunci](https://github.com/vanunci) e Leticia Murbach.

### Descrição

Esse trabalho vai se basear em um código inicialmente feito, onde existem algumas funções e structs previamente feitas e faz um “jogo” que executa sozinho. O código simula um rpg onde o jogador encontra 5 inimigos em sequência e os enfrenta em combate, ele vence se levar a vida de todos os inimigos para zero e perde se sua própria vida chegar a zero.

### Structs

O trabalho deverá conter pelo menos as seguintes `structs`:

- As `structs` do código disponibilizado, alterando jogador para armazenar a sua posição no mapa.

- Bloco: Essa `struct` representa um bloco no mapa, deve conter a informação se é bloqueado ou não (caminho ou pedra), se tem um inimigo ou não, e um ponteiro para uma variável do tipo `Inimigo` (que pode ficar vazio se não tiver nenhum inimigo no bloco).

- Mapa: Essa `struct` tem que ter uma altura `A`, uma largura `L`, e uma matriz de variáveis do tipo Bloco `AxL` (essa matriz deve ser alocada dinamicamente, os valores não devem vir de `template`).

- Fase: Deve conter um nome, uma variável do tipo `Mapa`, um número de inimigos `N` e um vetor de tamanho `N` de variáveis do tipo `Inimigo` (também alocado dinamicamente).

### Lógica

O trabalho deverá conter pelo menos as seguintes funções:

- `Mapa CriarMapa(int altura, int largura)`: Essa função deve receber a altura e largura do mapa e deve retornar uma nova variável desse tipo alocada dinamicamente com o tamanho passado por parâmetro (essa alocação deve ser pelo método utilizando memória contínua). Cada bloco desse mapa deve ter 20% de chance de ser bloqueado (pedra) e 80% de não ser bloqueado (caminho) e começar sem inimigos.

- `Fase CriarFase(int numInimigos, Inimigos* inimigos, int alturaMapa, int larguraMapa)`: Essa função deve receber por parâmetro o número de inimigos e um vetor de inimigos e também a altura e largura do mapa. Ela deve chamar a função `CriarMapa` para gerar o mapa. Depois ela deve, para cada inimigo, adicionar esse inimigo como membro de um bloco aleatório do mapa, desde que ele não esteja bloqueado e ainda não tenha nenhum inimigo. No final todos os inimigos devem estar alocados em posições no mapa, então se a posição sorteada for inválida, deve ser sorteada uma nova.

- `void Movimentar(...)`: Essa função deve movimentar o jogador pelo mapa (o usuário deve utilizar o teclado para isso), ele não deve conseguir andar para fora do mapa ou para uma posição que seja bloqueada. Se ele andar para uma posição com um inimigo, deve ser iniciado um combate automático (utilizar função do código disponibilizado para um inimigo). O jogo acaba se o jogador ou o último inimigo morrer.

**Obs..:** O código disponibilizado deve ser utilizado como base, mas podem ser adicionadas funcionalidades e variáveis caso achem necessário, por exemplo, alguma forma de curar vida ou armas diferentes.
