#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********************************ESTRUTURAS***********************************/
enum Status
{
  INATIVO = 0,
  ATIVO
};

typedef struct SCliente
{
  int cpf;
  char nome[50], email[50], telefone[15];
  enum Status status;
} Cliente;

enum Generos
{
  AVENTURA = 0,
  COMEDIA,
  DRAMA,
  TERROR,
  ACAO,
  ROMANCE
};

enum Classificacoes
{
  LIVRE = 0,
  MAIS_10,
  MAIS_12,
  MAIS_14,
  MAIS_16,
  MAIS_18
};

typedef struct SFilme
{
  int cod;
  char nome[50];
  enum Generos genero;
  enum Classificacoes classificacao;
} Filme;

typedef struct SPlanoBasico
{
  float valorPlano, valorExcedente;
  int limFilmes;
} PlanoBasico;

typedef struct SPlanoPremium
{
  float valorPlano;
} PlanoPremium;

typedef struct SPlanos
{
  PlanoBasico basico;
  PlanoPremium premium;
} Planos;

enum Planos
{
  BASICO = 0,
  PREMIUM
};

typedef struct SCredito
{
  char numCartao[17];
} Credito;

typedef struct SDebito
{
  char numAgencia[5];
  char numConta[10];
} Debito;

union FormaPagamento
{
  Credito credito;
  Debito debito;
};

enum TipoPagamento
{
  DEBITO = 0,
  CREDITO = 1
};

typedef struct SPagamento
{
  union FormaPagamento formaDePagamento;
  enum TipoPagamento tipoPagamento;
} Pagamento;

typedef struct SContrato
{
  int cpfCliente;
  int diaContratacao, mesContratacao;
  int diaCancelamento, mesCancelamento;
  enum Planos plano;
  Pagamento pagamento;
} Contrato;

typedef struct SHistoricoMensal
{
  int codFilme, data;
} HistoricoMensal;

typedef enum EErros
{
  CPF_JA_CADASTRADO = 0,
  CLIENTE_NAO_CADASTRADO,
  FILME_NAO_ENCONTRADO,
  CONTRATO_EXISTENTE,
  CLIENTE_INATIVO,
  TAXA_NEGADA,
  DATA_INVALIDA,
  LIMITE_FILMES_ATINGIDO,
  NENHUM_FILME_ASSISTIDO,
  NENHUM_CLIENTE_EXCEDENTE,
  NENHUM_CLIENTE_CADASTRADO,
  NENHUM_CONTRATO_CADASTRADO,
  TODOS_FILMES_ASSISTIDOS
} Erros;

typedef enum ESucesso
{
  OPERACAO_SUCEDIDA = 100
} Sucesso;

/********************************PROTOTIPOS************************************/
int cadastrarCliente(Cliente *clientes, int tamClientes, int *ctClientes);
int verificarCpfCliente(Cliente *clientes, int ctClientes, int cpf, int verStatus, int *posCliente, int *existe);
void cadastrarFilme(Filme *filmes, int tamFilmes, int *ctFilmes, int *codBaseFilmes, int *freqAbs);
void cadastrarPlanoBasico(Planos *planos);
void cadastrarPlanoPremium(Planos *planos);
int cadastrarContrato(Contrato *contratos, int *ctContratos, Cliente *clientes, int ctClientes, int mes);
int carregarFilme(Cliente *clientes, int ctClientes, Filme *filmes, int ctFilmes, Contrato *contratos, int mes,
                  HistoricoMensal *historico, int qtdFilmes, Planos planos, int *freqAbs, int *freqAbsTotal);
float calcularValorDevido(Contrato *contratos, int cpf, int ctClientes, int ctFilmes, enum Planos plano, Planos planos);
int gerarFatura(Cliente *clientes, int tamClientes, int ctClientes, HistoricoMensal *historico, int qtdFilmes,
                int *mes, Filme *filmes, int ctFilmes, Contrato *contratos, int ctContratos, Planos planos);
int cancelarContrato(Contrato *contratos, Cliente *clientes, int ctClientes, int mes, HistoricoMensal *historico, int qtdFilmes, Planos planos);
int listarDadosCliente(Cliente *clientes, int ctClientes);
void obterDadosFilme(int eGenero, int eClassificacao, char *genero, char *classificacao);
int listarHistoricoCliente(Cliente *clientes, int ctClientes, HistoricoMensal *historico,
                           int qtdFilmes, Filme *filmes, int ctFilmes, int mes);
int listarClientesPorCota(Cliente *clientes, int ctClientes, Contrato *contratos, int ctContratos, HistoricoMensal *historico, int qtdFilmes, Planos planos);
int obterFrequenciaFilme(Filme *filmes, int ctFilmes, int *freqAbs, int freqAbsTotal);
int recomendarFilme(Cliente *clientes, int ctClientes, HistoricoMensal *historico, int qtdFilmes, Filme *filmes, int ctFilmes);

/**********************************MAIN****************************************/
int main(void)
{
  //CONSTANTES
  const int tamClientes = 10, tamFilmes = 10, qtdFilmes = 30;

  //VARIAVEIS AUXILIARES
  int i, j, op, res;

  //CONTADORES
  int mes = 1; //começa em Janeiro
  int ctClientes = 0, ctFilmes = 0, ctContratos = 0;
  int codBaseFilmes = 1000;
  int freqAbsTotal = 0;

  //VETORES
  Cliente clientes[tamClientes];
  Filme filmes[tamFilmes];
  Contrato contratos[tamClientes];
  HistoricoMensal historico[tamClientes][qtdFilmes];
  int freqAbs[tamFilmes];

  //PLANOS (PREMIUM e BÁSICO)
  Planos planos;

  //Inicializando a matriz de histórico mensal...
  for (i = 0; i < tamClientes; i++)
    for (j = 0; j < qtdFilmes; j++)
    {
      historico[i][j].codFilme = 0;
      historico[i][j].data = 0;
    }

  do
  {
    /*
    printf("*--------------Bem Vindo ao Sistema de Streaming--------------*\n");
    printf("1. Cadastrar de Cliente\n");
    printf("2. Cadastrar Filme\n");
    printf("3. Cadastrar plano basico\n");
    printf("4. Cadastrar plano premium\n");
    printf("5. Cadastrar contrato\n");
    printf("6. Carregar filme\n");
    printf("7. Cancelar contrato\n");
    printf("8. Gerar fatura\n");
    printf("9. Listar os dados de um dado cliente\n");
    printf("10. Listar o historico de uma dado cliente\n");
    printf("11. Listar clientes que excederam a cota mensal\n");
    printf("12. Frequencia de um dado filme\n");
    printf("13. Recomendar filme\n");
    printf("0. Sair\n");
    printf("Digite a opcao desejada (0 - 13): ");
    */

    scanf("%d", &op);

    switch (op)
    {
    //Cadastrar Clientes
    case 1:
    {
      if (ctClientes == tamClientes)
        printf("ERRO: Numero maximo de clientes no sistema atingido\n");
      else
      {
        res = cadastrarCliente(clientes, tamClientes, &ctClientes);
        if (res == CPF_JA_CADASTRADO)
          printf("ERRO: CPF do cliente ja cadastrado\n");
        else if (res == OPERACAO_SUCEDIDA)
          printf("Cliente cadastrado com sucesso\n");
      }
      break;
    }
    //Cadastrar Filmes
    case 2:
    {
      if (ctFilmes == tamFilmes)
        printf("ERRO: Numero maximo de filmes no sistema atingido\n");
      else
      {
        cadastrarFilme(filmes, tamFilmes, &ctFilmes, &codBaseFilmes, freqAbs);
        printf("Filme cadastrado com sucesso\n");
      }
      break;
    }
    //Cadastrar Plano Básico
    case 3:
    {
      cadastrarPlanoBasico(&planos);
      printf("Plano Basico cadastrado com sucesso\n");
      break;
    }
    //Cadastrar Plano Premium
    case 4:
    {
      cadastrarPlanoPremium(&planos);
      printf("Plano Premium cadastrado com sucesso\n");
      break;
    }
    //Cadastrar Contrato
    case 5:
    {
      if (ctClientes == 0)
        printf("ERRO: Nenhum cliente cadastrado no sistema\n");
      else if (ctContratos == tamClientes)
        printf("ERRO: Numero maximo de contratos no sistema atingido\n");
      else
      {
        res = cadastrarContrato(contratos, &ctContratos, clientes, ctClientes, mes);
        if (res == CLIENTE_NAO_CADASTRADO)
          printf("ERRO: Cliente nao cadastrado\n");
        else if (res == CONTRATO_EXISTENTE)
          printf("ERRO: Cliente com contrato existente\n");
        else if (res == OPERACAO_SUCEDIDA)
          printf("Contrato cadastrado com sucesso\n");
      }
      break;
    }
    //Carregar filme
    case 6:
    {
      if (ctClientes == 0)
      {
        printf("ERRO: Nenhum cliente cadastrado no sistema\n");
      }
      else
      {
        res = carregarFilme(clientes, ctClientes, filmes, ctFilmes, contratos, mes, &historico[0][0],
                            qtdFilmes, planos, freqAbs, &freqAbsTotal);

        if (res == CLIENTE_NAO_CADASTRADO)
          printf("ERRO: Cliente nao cadastrado\n");
        else if (res == CLIENTE_INATIVO)
          printf("ERRO: Cliente nao ativo\n");
        else if (res == FILME_NAO_ENCONTRADO)
          printf("ERRO: Filme nao encontrado\n");
        else if (res == LIMITE_FILMES_ATINGIDO)
          printf("ERRO: Numero maximo de filmes no sistema atingido\n");
        else if (res == TAXA_NEGADA)
          printf("ERRO: Taxa adicional negada\n");
        else if (res == OPERACAO_SUCEDIDA)
          printf("Filme carregado com sucesso\n");
      }
      break;
    }
    //Cancelar contrato
    case 7:
    {
      if (ctContratos == 0)
        printf("ERRO: Nenhum contrato cadastrado no sistema\n");
      else
      {
        res = cancelarContrato(contratos, clientes, ctClientes, mes, &historico[0][0], qtdFilmes, planos);
        if (res == CLIENTE_NAO_CADASTRADO)
          printf("ERRO: Cliente nao cadastrado\n");
        else if (res == CLIENTE_INATIVO)
          printf("ERRO: Cliente inativo\n");
        else if (res == DATA_INVALIDA)
          printf("ERRO: Data de cancelamente anteiror a data de contratacao\n");
        else if (res == OPERACAO_SUCEDIDA)
          printf("Cancelamento feito com sucesso\n");
      }
      break;
    }
    //Gerar fatura
    case 8:
    {
      res = gerarFatura(clientes, tamClientes, ctClientes, &historico[0][0], qtdFilmes, &mes, filmes, ctFilmes, contratos, ctContratos, planos);
      if (res == NENHUM_CONTRATO_CADASTRADO)
        printf("ERRO: Nenhum contrato cadastrado no sistema\n");
      else if (res == NENHUM_CLIENTE_CADASTRADO)
        printf("ERRO: Nenhum cliente cadastrado no sistema\n");
      else if (res == CLIENTE_NAO_CADASTRADO)
        printf("ERRO: Cliente nao cadastrado\n");
      else if (res == CLIENTE_INATIVO)
        printf("ERRO: Cliente nao possui contrato\n");
      break;
    }
    //Listar dados dos clientes
    case 9:
    {
      if (ctClientes == 0)
        printf("ERRO: Nenhum cliente cadastrado no sistema\n");
      else
      {
        res = listarDadosCliente(clientes, ctClientes);
        if (res == CLIENTE_NAO_CADASTRADO)
          printf("ERRO: Cliente nao cadastrado\n");
      }
      break;
    }
    //Listar histórico de um cliente
    case 10:
    {
      if (ctClientes == 0)
        printf("ERRO: Nenhum cliente cadastrado no sistema\n");
      else
      {
        res = listarHistoricoCliente(clientes, ctClientes, &historico[0][0], qtdFilmes, filmes, ctFilmes, mes);
        if (res == CLIENTE_NAO_CADASTRADO)
          printf("ERRO: Cliente nao cadastrado\n");
        else if (res == NENHUM_FILME_ASSISTIDO)
          printf("ERRO: Nenhum filme assistido\n");
      }
      break;
    }
    //Listar clientes excedentes da cota mensal
    case 11:
    {
      res = listarClientesPorCota(clientes, ctClientes, contratos, ctContratos, &historico[0][0], qtdFilmes, planos);
      if (res == NENHUM_CLIENTE_EXCEDENTE)
        printf("ERRO: Nenhum cliente excedente\n");
      break;
    }
    //Obter a frequência de um dado filme
    case 12:
    {
      if (ctFilmes == 0)
        printf("ERRO: Nenhum filme cadastrado no sistema\n");
      else
      {
        res = obterFrequenciaFilme(filmes, ctFilmes, freqAbs, freqAbsTotal);
        if (res == FILME_NAO_ENCONTRADO)
          printf("ERRO: Codigo invalido\n");
        else if (res == NENHUM_FILME_ASSISTIDO)
          printf("ERRO: Nenhum filme assistido\n");
      }
      break;
    }
    //Recomendar Filme
    case 13:
    {
      if (ctClientes == 0)
        printf("ERRO: Nenhum cliente cadastrado no sistema\n");
      else
      {
        res = recomendarFilme(clientes, ctClientes, &historico[0][0], qtdFilmes, filmes, ctFilmes);
        if (res == CLIENTE_NAO_CADASTRADO)
          printf("ERRO: Cliente nao cadastrado\n");
        else if (res == CLIENTE_INATIVO)
          printf("ERRO: Cliente nao ativo\n");
        else if (res == NENHUM_FILME_ASSISTIDO)
          printf("ERRO: Nenhum filme assistido\n");
        else if (res == TODOS_FILMES_ASSISTIDOS)
          printf("ERRO: Todos os filmes do(s) genero(s) foram assistidos\n");
      }
    }
    break;
    case 0:
      printf("Finalizando programa...");
      break;
    default:
      printf("ERRO: Opcao invalida\n");
    }
  } while (op != 0);
}

/*********************************FUNCOES**************************************/
int cadastrarCliente(Cliente *clientes, int tamClientes, int *ctClientes)
{
  int i;
  int cpf;
  /*
  printf("*----------------Cadastro de Cliente----------------*\n");
  printf("Digite o numero de CPF: ");
  */

  scanf(" %d", &cpf);

  //Verificar se o CPF digitado está cadastrado no vetor de CLIENTES
  for (i = 0; i < tamClientes; i++)
    if (clientes[i].cpf == cpf)
      return CPF_JA_CADASTRADO;

  clientes[*ctClientes].cpf = cpf;

  //printf("Digite o nome: ");
  scanf(" %[^\n]%*c", clientes[*ctClientes].nome);

  //printf("Digite o endereco de e-mail: ");
  scanf(" %s", clientes[*ctClientes].email);

  //printf("Digite o numero de telefone: ");
  scanf(" %s", clientes[*ctClientes].telefone);

  clientes[*ctClientes].status = INATIVO;
  (*ctClientes)++;
  return OPERACAO_SUCEDIDA;
}

int verificarCpfCliente(Cliente *clientes, int ctClientes, int cpf, int verStatus, int *posCliente, int *existe)
{
  int i;

  //Verificar se o CPF está cadastrado no vetor CLIENTES
  for (i = 0; i < ctClientes; i++)
    if (clientes[i].cpf == cpf)
    {
      *posCliente = i;
      *existe = 1;

      //Verificar se o cliente tem um plano INATIVO
      if (verStatus)
      {
        if (clientes[i].status == INATIVO)
          return CLIENTE_INATIVO;
      }

      break;
    }

  return OPERACAO_SUCEDIDA;
}

void cadastrarFilme(Filme *filmes, int tamFilmes, int *ctFilmes, int *codBaseFilmes, int *freqAbs)
{
  int genero, classificacao;
  /*
  printf("*----------------Cadastro de Filme----------------*\n");
  printf("Digite o nome: ");
  */

  scanf(" %[^\n]%*c", filmes[*ctFilmes].nome);

  //Looping até o usuário digitar um genêro válido
  do
  {
    //printf("Digite o genero: (0 - 5): ");
    scanf("%d", &genero);
    if (genero < AVENTURA || genero > ROMANCE)
      printf("ERRO: Genero invalido\n");
  } while (genero < AVENTURA || genero > ROMANCE);

  filmes[*ctFilmes].genero = (enum Generos)genero;

  //Looping até o usuário digitar um classficação válida
  do
  {
    //printf("Digite a classificacao: (0 - 5): ");
    scanf("%d", &classificacao);
    if (classificacao < LIVRE || classificacao > MAIS_18)
      printf("ERRO: Classificao invalida\n");
  } while (classificacao < LIVRE || classificacao > MAIS_18);

  filmes[*ctFilmes].classificacao = (enum Classificacoes)classificacao;

  //Auto-incrementação do código do filme, a partir de um código base
  filmes[*ctFilmes].cod = *codBaseFilmes + (*ctFilmes + 1);
  freqAbs[*ctFilmes] = 0;

  (*ctFilmes)++;
}

void cadastrarPlanoBasico(Planos *planos)
{
  //printf("*----------------Cadastro do Plano Basico----------------*\n");

  //printf("Digite a quantidade de filmes que o usuario podera assistir por mes: ");
  scanf("%d", &planos->basico.limFilmes);

  //printf("Digite o valor base do plano: ");
  scanf("%f", &planos->basico.valorPlano);

  //printf("Digite o valor excedente por filme adicional assistido: ");
  scanf("%f", &planos->basico.valorExcedente);
}

void cadastrarPlanoPremium(Planos *planos)
{
  //printf("*----------------Cadastro do Plano Premium----------------*\n");

  //printf("Digite o valor base do plano: ");
  scanf("%f", &planos->premium.valorPlano);
}

int cadastrarContrato(Contrato *contratos, int *ctContratos, Cliente *clientes, int ctClientes, int mes)
{
  int i, existe = 0;
  int posCliente, posContrato;
  int cpf, plano, tipoPagamento, dia;
  /*
  printf("*----------------Cadastro de Contrato----------------*\n");
  printf("Digite o CPF do cliente associado: ");
  */

  scanf("%d", &cpf);

  verificarCpfCliente(clientes, ctClientes, cpf, 0, &posCliente, &existe);

  //Se o CPF não existir no vetor de CLIENTES, retorna que não existe
  if (!existe)
    return CLIENTE_NAO_CADASTRADO;

  posContrato = *ctContratos;
  existe = 0;

  //Verificar se o CPF já está vinculado a um contrato
  for (i = 0; i < ctClientes; i++)
    if (contratos[i].cpfCliente == cpf)
    {
      if (clientes[posCliente].status == ATIVO)
        return CONTRATO_EXISTENTE;
      else
      {
        posContrato = i;
        existe = 1;
        break;
      }
    }

  contratos[posContrato].cpfCliente = cpf;

  do
  {
    //printf("Digite o tipo de plano (0-1): ");
    scanf("%d", &plano);

    if (plano < BASICO || plano > PREMIUM)
      printf("ERRO: Tipo de plano invalido\n");

  } while (plano < BASICO || plano > PREMIUM);

  contratos[posContrato].plano = (enum Planos)plano;

  do
  {
    //printf("Digite o tipo de pagamento (0-1): ");
    scanf("%d", &tipoPagamento);

    if (tipoPagamento < DEBITO || tipoPagamento > CREDITO)
      printf("ERRO: Tipo de pagamento invalido\n");

  } while (tipoPagamento < DEBITO || tipoPagamento > CREDITO);

  //Inserir o tipo de pagamento escolhido na enumeração
  contratos[posContrato].pagamento.tipoPagamento = (enum TipoPagamento)tipoPagamento;

  //Cadastrar o tipo de pagamento
  switch (contratos[posContrato].pagamento.tipoPagamento)
  {
  case DEBITO:
  {
    //printf("Digite o numero da agencia: ");
    scanf(" %s", contratos[posContrato].pagamento.formaDePagamento.debito.numAgencia);
    //printf("Digite o numero da conta: ");
    scanf(" %s", contratos[posContrato].pagamento.formaDePagamento.debito.numConta);
    break;
  }

  case CREDITO:
    //printf("Digite o numero do cartao: ");
    scanf(" %s", contratos[posContrato].pagamento.formaDePagamento.credito.numCartao);
  }

  //Cadastrar o dia de contratacao e verificar se é um dia válido (1 - 31 dias)
  do
  {
    //printf("Digite o dia de contratacao: ");
    scanf("%d", &dia);

    if (dia < 1 || dia > 31)
      printf("ERRO: Dia invalido\n");

  } while (dia < 1 || dia > 31);

  contratos[posContrato].diaContratacao = dia;
  contratos[posContrato].mesContratacao = mes;

  //Inicialmente, a data de cancelamento é 0
  contratos[posContrato].diaCancelamento = 0;
  contratos[posContrato].mesCancelamento = 0;

  //Mudar o status do cliente para ATIVO
  clientes[posCliente].status = ATIVO;

  if (!existe)
    (*ctContratos)++;

  return OPERACAO_SUCEDIDA;
}

int carregarFilme(Cliente *clientes, int ctClientes, Filme *filmes, int ctFilmes, Contrato *contratos, int mes, HistoricoMensal *historico, int qtdFilmes, Planos planos, int *freqAbs, int *freqAbsTotal)
{
  int i, existe = 0, res, op;
  int posCliente, posContrato, posFilme, posHistorico;
  int ctPlanoBasico = 0;
  int cpf, dia, codFilme, auxFilme;

  /*
  printf("*----------------Carregar Filme----------------*\n");
  printf("Digite o numero de CPF: ");
  */

  scanf("%d", &cpf);

  res = verificarCpfCliente(clientes, ctClientes, cpf, 1, &posCliente, &existe);

  //Se o CPF não existir no vetor de CLIENTES, retorna que não existe
  if (!existe)
    return CLIENTE_NAO_CADASTRADO;

  if (res == CLIENTE_INATIVO)
    return CLIENTE_INATIVO;

  existe = 0;
  //Verificar se há a possibilidade do cliente carregar um filme
  for (i = 0; i < qtdFilmes; i++)
    if ((historico + (posCliente * qtdFilmes) + i)->codFilme == 0)
    {
      existe = 1;
      break;
    }

  //Caso não haja, significa que o cliente atingiu o limite de filmes por mês
  if (!existe)
    return LIMITE_FILMES_ATINGIDO;

  do
  {
    //printf("Digite o dia de carregamento do filme: ");
    scanf("%d", &dia);

    if (dia < 1 || dia > 31)
      printf("ERRO: Dia invalido\n");
  } while (dia < 1 || dia > 31);

  do
  {
    //printf("Digite por qual meio deseja pesquisar o filme (0 - Genero / 1 - Classificacao): ");
    scanf("%d", &op);

    if (op < 0 || op > 1)
      printf("ERRO: Escolha invalida\n");
  } while (op < 0 || op > 1);

  do
  {
    scanf("%d", &auxFilme);
    if (auxFilme < 0 || auxFilme > 5)
      printf("ERRO: Escolha invalida\n");
  } while (auxFilme < 0 || auxFilme > 5);

  for (i = 0; i < ctFilmes; i++)
  {
    //Listar os filmes por classificação escolhido pelo cliente
    if (op == 0)
    {
      if (filmes[i].genero == auxFilme)
      {
        printf("Codigo: %d\n", filmes[i].cod);
        printf("Nome: %s\n", filmes[i].nome);
      }
    }
    //Listar os filmes pelo genêro escolhido pelo cliente
    else if (op == 1)
    {
      if (filmes[i].classificacao == auxFilme)
      {
        printf("Codigo: %d\n", filmes[i].cod);
        printf("Nome: %s\n", filmes[i].nome);
      }
    }
  }

  //printf("Digite o codigo do filme desejado: ");
  scanf("%d", &codFilme);

  //Verificar se o código do filme digitado está cadastrado no vetor FILMES
  existe = 0;
  for (i = 0; i < ctFilmes; i++)
    if (filmes[i].cod == codFilme)
    {
      posFilme = i;
      existe = 1;
      break;
    }

  //Se o código do filme não estiver cadastrado, retorna que não existe
  if (!existe)
    return FILME_NAO_ENCONTRADO;

  //Obter o plano do cliente
  for (i = 0; i < ctClientes; i++)
    if (contratos[i].cpfCliente == cpf)
    {
      posContrato = i;
      break;
    }

  //Realizar ações, de acordo com o plano do cliente
  switch (contratos[posContrato].plano)
  {
  case PREMIUM:
  {
    //Pegar a posição que o filme poderá ser alocado na matriz HISTORICO
    for (i = 0; i < qtdFilmes; i++)
    {
      if ((historico + (posCliente * qtdFilmes) + i)->codFilme == 0)
      {
        posHistorico = i;
        break;
      }
    }
    break;
  }
  case BASICO:
  {
    //Verificar quantos filmes o cliente já assistiu
    for (i = 0; i < qtdFilmes; i++)
    {
      if ((historico + (posCliente * qtdFilmes) + i)->codFilme == 0)
      {
        posHistorico = i;
        break;
      }
      else
        ctPlanoBasico++;
    }

    //Irá tomar uma ação, se o cliente atingiu o limite do plano básico
    if (ctPlanoBasico >= planos.basico.limFilmes)
    {
      do
      {
        //printf("ERRO: Limite excedido! Deseja continuar assistindo, pagando uma taxa adicional? (0 - Nao / 1 - Sim): ");
        scanf("%d", &op);

        if (op < 0 || op > 1)
          printf("ERRO: Escolha invalida\n");

        if (!op)
          return TAXA_NEGADA;
      } while (op < 0 || op > 1);
    }
    break;
  }
  }

  //Salvar as informações no histórico mensal do cliente
  (historico + (posCliente * qtdFilmes) + posHistorico)->codFilme = codFilme;
  (historico + (posCliente * qtdFilmes) + posHistorico)->data = dia;

  //Incrementar em +1 a frequência absoluta de um dado filme
  freqAbs[posFilme] = freqAbs[posFilme] + 1;
  //Incrementar em +1 a frequência absoluta total
  (*freqAbsTotal)++;

  return OPERACAO_SUCEDIDA;
}

int cancelarContrato(Contrato *contratos, Cliente *clientes, int ctClientes, int mes, HistoricoMensal *historico, int qtdFilmes, Planos planos)
{
  int i, existe = 0, res;
  int posCliente, posContrato;
  int cpf, dia, planoCliente, codFilme, ctFilmes = 0;
  float valor;
  /*
  printf("*----------------Cancelar Contrato----------------*\n");
  printf("Digite o numero de CPF: ");
  */

  scanf("%d", &cpf);

  res = verificarCpfCliente(clientes, ctClientes, cpf, 1, &posCliente, &existe);

  if (!existe)
    return CLIENTE_NAO_CADASTRADO;

  if (res == CLIENTE_INATIVO)
    return CLIENTE_INATIVO;

  //Obter o plano do cliente e a posição do cliente no vetor CONTRATOS
  for (i = 0; i < ctClientes; i++)
    if (contratos[i].cpfCliente == cpf)
    {
      posContrato = i;
      planoCliente = contratos[i].plano;
      break;
    }

  do
  {
    //printf("Digite o dia de carregamento do filme: ");
    scanf("%d", &dia);

    if (dia < 1 || dia > 31)
      printf("ERRO: Dia invalido\n");

  } while (dia < 1 || dia > 31);

  if (dia < contratos[posContrato].diaContratacao && contratos[posContrato].mesContratacao == mes)
    return DATA_INVALIDA;

  //Atualizar data de cancelamento
  contratos[posContrato].diaCancelamento = dia;
  contratos[posContrato].mesCancelamento = mes;

  //Atualizar data de contratação
  contratos[posContrato].diaContratacao = 0;
  contratos[posContrato].mesContratacao = 0;

  clientes[posCliente].status = INATIVO;

  //Obter quantos filmes o cliente de plano BASICO assistiu
  if (planoCliente == BASICO)
    for (i = 0; i < qtdFilmes; i++)
    {
      codFilme = (historico + (posCliente * qtdFilmes) + i)->codFilme;

      if (codFilme == 0)
        break;

      ctFilmes++;
    }

  valor = calcularValorDevido(contratos, cpf, ctClientes, ctFilmes, planoCliente, planos);

  printf("Valor devido: %.2f\n", valor);

  return OPERACAO_SUCEDIDA;
}

float calcularValorDevido(Contrato *contratos, int cpf, int ctClientes, int ctFilmes, enum Planos plano, Planos planos)
{
  float valor;

  switch (plano)
  {
  case PREMIUM:
    valor = planos.premium.valorPlano;
    break;
  case BASICO:
  {
    if (ctFilmes >= planos.basico.limFilmes)
    {
      if ((ctFilmes - planos.basico.limFilmes) == 0)
        ctFilmes++;
      valor = ((ctFilmes - planos.basico.limFilmes) * planos.basico.valorExcedente) + planos.basico.valorPlano;
    }
    else
      valor = planos.basico.valorPlano;
    break;
  }
  }

  return valor;
}

int gerarFatura(Cliente *clientes, int tamClientes, int ctClientes, HistoricoMensal *historico, int qtdFilmes, int *mes, Filme *filmes, int ctFilmes, Contrato *contratos, int ctContratos, Planos planos)
{
  int i, j, op, res, existe = 0;
  int posCliente, posFilme;
  int codFilme, ctFilmesLocal = 0;
  enum Planos planoCliente;
  int cpf;
  float valor;

  //printf("*----------------Gerar Fatura----------------*\n");

  do
  {
    /*
    printf("0. Gerar uma fatura especifica\n");
    printf("1. Gerar a fatura de todos os clientes\n");
    printf("Digite a opcao desejada: ");
    */

    scanf("%d", &op);
    if (op < 0 || op > 1)
      printf("ERRO: Escolha invalida\n");
  } while (op < 0 || op > 1);

  switch (op)
  {
  //Gerar uma fatura específica
  case 0:
  {
    if (ctClientes == 0)
      return NENHUM_CLIENTE_CADASTRADO;

    if (ctContratos == 0)
      return NENHUM_CONTRATO_CADASTRADO;

    //printf("Digite o CPF: ");
    scanf("%d", &cpf);

    res = verificarCpfCliente(clientes, ctClientes, cpf, 1, &posCliente, &existe);

    if (!existe)
      return CLIENTE_NAO_CADASTRADO;

    if (res == CLIENTE_INATIVO)
      return CLIENTE_INATIVO;

    //Procurar o código do contrato dado um cliente específico e pegar seu plano
    for (i = 0; i < ctContratos; i++)
      if (contratos[i].cpfCliente == cpf)
      {
        planoCliente = contratos[i].plano;
        break;
      }

    for (i = 0; i < qtdFilmes; i++)
    {
      codFilme = (historico + (posCliente * qtdFilmes) + i)->codFilme;

      if (codFilme == 0)
        break;

      for (j = 0; j < ctFilmes; j++)
        if (codFilme == filmes[j].cod)
        {
          posFilme = j;
          break;
        }

      printf("Nome: %s\n", filmes[posFilme].nome);
      printf("Data: %d/%d\n", (historico + (posCliente * qtdFilmes) + i)->data, *mes);
      ctFilmesLocal++;
    }

    valor = calcularValorDevido(contratos, cpf, ctClientes, ctFilmesLocal, planoCliente, planos);
    printf("Valor devido: %.2f\n", valor);

    break;
  }

  //Gerar as faturas para todos os clientes
  case 1:
  {
    for (i = 0; i < ctClientes; i++)
    {
      if (clientes[i].status == ATIVO)
      {
        printf("CPF: %d\n", clientes[i].cpf);
        printf("Nome: %s\n", clientes[i].nome);

        //Procurar os códigos dos contratos dos clientes e pegar seus planos
        for (j = 0; j < ctContratos; j++)
          if (clientes[i].cpf == contratos[j].cpfCliente)
          {
            planoCliente = contratos[j].plano;
            break;
          }

        //Se o plano for básico, contabilizar quantos filmes o cliente assistiu
        if (planoCliente == BASICO)
        {
          for (j = 0; j < ctFilmes; j++)
          {
            codFilme = (historico + (i * qtdFilmes) + j)->codFilme;
            if (codFilme == 0)
              break;

            ctFilmesLocal++;
          }
        }

        valor = calcularValorDevido(contratos, cpf, ctClientes, ctFilmesLocal, planoCliente, planos);
        printf("Valor devido: %.2f\n", valor);
      }
    }

    //Limpar histórico mensal de todos os clientes
    for (i = 0; i < tamClientes; i++)
    {
      for (j = 0; j < qtdFilmes; j++)
      {
        if ((historico + (i * qtdFilmes) + j)->codFilme != 0)
        {
          (historico + (i * qtdFilmes) + j)->codFilme = 0;
          (historico + (i * qtdFilmes) + j)->data = 0;
        }
        else
          break;
      }

      (*mes)++;
      printf("Mes vigente apos a fatura: %d\n", *mes);
      break;
    }
  }
  }
  return OPERACAO_SUCEDIDA;
}

int listarDadosCliente(Cliente *clientes, int ctClientes)
{
  int existe = 0, posCliente;
  int cpf;
  /*
  printf("*----------------Listar Dados do Cliente----------------*\n");
  printf("Digite o numero de CPF: ");
  */

  scanf("%d", &cpf);

  verificarCpfCliente(clientes, ctClientes, cpf, 0, &posCliente, &existe);

  if (!existe)
    return CLIENTE_NAO_CADASTRADO;

  printf("Nome: %s\n", clientes[posCliente].nome);
  printf("Email: %s\n", clientes[posCliente].email);
  printf("Telefone: %s\n", clientes[posCliente].telefone);
  printf("Status: %d\n", clientes[posCliente].status);

  return OPERACAO_SUCEDIDA;
}

void obterDadosFilme(int eGenero, int eClassificacao, char *genero, char *classificacao)
{
  switch (eGenero)
  {
  case AVENTURA:
    strcpy(genero, "aventura");
    break;
  case COMEDIA:
    strcpy(genero, "comedia");
    break;
  case DRAMA:
    strcpy(genero, "drama");
    break;
  case TERROR:
    strcpy(genero, "terror");
    break;
  case ACAO:
    strcpy(genero, "acao");
    break;
  case ROMANCE:
    strcpy(genero, "romance");
    break;
  }

  switch (eClassificacao)
  {
  case LIVRE:
    strcpy(classificacao, "livre");
    break;
  case MAIS_10:
    strcpy(classificacao, "+10");
    break;
  case MAIS_12:
    strcpy(classificacao, "+12");
    break;
  case MAIS_14:
    strcpy(classificacao, "+14");
    break;
  case MAIS_16:
    strcpy(classificacao, "+16");
    break;
  case MAIS_18:
    strcpy(classificacao, "+18");
    break;
  }
}

int listarHistoricoCliente(Cliente *clientes, int ctClientes, HistoricoMensal *historico, int qtdFilmes, Filme *filmes, int ctFilmes, int mes)
{
  int i, j, existe = 0, posCliente;
  int cpf, codFilme, dia;
  char genero[50], classificacao[50];

  /*
  printf("*----------------Listar Historico do Cliente----------------*\n");
  printf("Digite o numero de CPF: ");
  */
  scanf("%d", &cpf);

  verificarCpfCliente(clientes, ctClientes, cpf, 0, &posCliente, &existe);

  if (!existe)
    return CLIENTE_NAO_CADASTRADO;

  if (clientes[posCliente].status == ATIVO)
    printf("Estado: Ativo\n");
  else if (clientes[posCliente].status == INATIVO)
    printf("Estado: Inativo\n");

  //Se a posição [posCliente][0].codFilme da matriz histórico for 0
  //O cliente não assistiu nenhum filme
  if ((historico + (posCliente * qtdFilmes) + 0)->codFilme == 0)
    return NENHUM_FILME_ASSISTIDO;

  for (i = 0; i < qtdFilmes; i++)
  {
    codFilme = (historico + (posCliente * qtdFilmes) + i)->codFilme;

    if (codFilme == 0)
      break;

    dia = (historico + (posCliente * qtdFilmes) + i)->data;
    for (j = 0; j < ctFilmes; j++)
    {
      if (codFilme == filmes[j].cod)
      {
        printf("Codigo: %d\n", codFilme);
        printf("Nome: %s\n", filmes[j].nome);
        obterDadosFilme(filmes[j].genero, filmes[j].classificacao, genero, classificacao);
        printf("Genero: %s\n", genero);
        printf("Classificacao: %s\n", classificacao);
        printf("Data do carregamento: %d/%d\n\n", dia, mes);
        break;
      }
    }
  }

  return OPERACAO_SUCEDIDA;
}

int listarClientesPorCota(Cliente *clientes, int ctClientes, Contrato *contratos, int ctContratos, HistoricoMensal *historico, int qtdFilmes, Planos planos)
{
  int i, j, existe = 0;
  int posCliente, ctFilmes = 0;
  int codFilme;

  //printf("*----------------Listar Clientes Excedentes da Cota Mensal----------------*\n");

  for (i = 0; i < ctContratos; i++)
  {
    ctFilmes = 0;
    //Verificar se o cliente tem o plano básico e se está ativo
    if (contratos[i].plano == BASICO && contratos[i].diaCancelamento == 0)
    {
      //Obter a posição do cliente do vetor CLIENTES
      for (j = 0; j < ctClientes; j++)
        if (contratos[i].cpfCliente == clientes[j].cpf)
        {
          posCliente = j;
          break;
        }

      //Verificar quantos filmes os clientes assistiram
      for (j = 0; j < qtdFilmes; j++)
      {
        codFilme = (historico + (posCliente * qtdFilmes) + j)->codFilme;

        if (codFilme == 0)
          break;

        ctFilmes++;
      }

      //Fazer isso, caso tenha excedido o limite de filmes
      if (ctFilmes > planos.basico.limFilmes)
      {
        printf("CPF: %d\n", clientes[posCliente].cpf);
        printf("Nome: %s\n", clientes[posCliente].nome);
        printf("Valor excedente: %.2f\n\n", (float)((ctFilmes - planos.basico.limFilmes) * planos.basico.valorExcedente));
        existe = 1;
      }
    }
  }

  if (!existe)
    return NENHUM_CLIENTE_EXCEDENTE;

  return OPERACAO_SUCEDIDA;
}

int obterFrequenciaFilme(Filme *filmes, int ctFilmes, int *freqAbs, int freqAbsTotal)
{
  int i, existe = 0, posFilme;
  int cod;

  //printf("*----------------Obter a Frequencia de um Filme----------------*\n");

  //printf("Digite o codigo do filme: ");
  scanf("%d", &cod);

  for (i = 0; i < ctFilmes; i++)
    if (cod == filmes[i].cod)
    {
      posFilme = i;
      existe = 1;
      break;
    }

  if (!existe)
    return FILME_NAO_ENCONTRADO;

  if (freqAbsTotal == 0)
    return NENHUM_FILME_ASSISTIDO;

  printf("Frequencia: %.2f%%\n", (float)freqAbs[posFilme] / freqAbsTotal * 100);
  return OPERACAO_SUCEDIDA;
}

void imprimirFilmePorGenero(Filme *filmes, int ctFilmes, enum Generos tipoGenero, HistoricoMensal *historico, int posCliente, int qtdFilmes, char genero[50], char classificacao[50])
{
  int i, j, existe = 0;
  int codFilme;

  //Printar todos os filmes, de acordo com o gênero, que o cliente não assistiu
  for (i = 0; i < ctFilmes; i++)
  {
    if (tipoGenero == filmes[i].genero)
    {
      for (j = 0; j < qtdFilmes; j++)
      {
        codFilme = (historico + (posCliente * qtdFilmes) + j)->codFilme;

        if (codFilme == 0)
          break;

        if (codFilme == filmes[i].cod)
        {
          existe = 1;
          break;
        }
      }

      if (!existe)
      {
        printf("Codigo: %d\n", filmes[i].cod);
        printf("Nome: %s\n", filmes[i].nome);
        obterDadosFilme(filmes[i].genero, filmes[i].classificacao, genero, classificacao);
        printf("Genero: %s\n", genero);
        printf("Classificacao: %s\n", classificacao);
      }

      existe = 0;
    }
  }
}

int recomendarFilme(Cliente *clientes, int ctClientes, HistoricoMensal *historico, int qtdFilmes, Filme *filmes, int ctFilmes)
{
  int i, j, res, existe = 0;
  int posCliente;
  int cpf, codFilme;
  char genero[50], classificacao[50];

  const int qtdGeneros = 6;
  int generos[qtdGeneros];

  enum Generos *tipoGeneros;
  int tamGeneros = 1, ctGenero = 0, maiorGenero = 0;

  //Inicializando vetor de generos (0 - 5) com valor 0
  for (i = 0; i < qtdGeneros; i++)
    generos[i] = 0;

  tipoGeneros = (enum Generos *)malloc(sizeof(int) * tamGeneros);

  /*
  printf("*----------------Recomendar Filme----------------*\n");
  printf("Digite o CPF: ");
  */

  scanf("%d", &cpf);

  res = verificarCpfCliente(clientes, ctClientes, cpf, 1, &posCliente, &existe);

  if (!existe)
    return CLIENTE_NAO_CADASTRADO;

  if (res == CLIENTE_INATIVO)
    return CLIENTE_INATIVO;

  if ((historico + (posCliente * qtdFilmes) + 0)->codFilme == 0)
    return NENHUM_FILME_ASSISTIDO;

  //Obter de que gênero são os filmes que o clientes assistiu
  for (i = 0; i < qtdFilmes; i++)
  {
    codFilme = (historico + (posCliente * qtdFilmes) + i)->codFilme;

    if (codFilme == 0)
      break;

    for (j = 0; j < ctFilmes; j++)
    {
      if (codFilme == filmes[j].cod)
      {
        switch (filmes[j].genero)
        {
        case AVENTURA:
          generos[AVENTURA] = generos[AVENTURA] + 1;
          break;
        case COMEDIA:
          generos[COMEDIA] = generos[COMEDIA] + 1;
          break;
        case DRAMA:
          generos[DRAMA] = generos[DRAMA] + 1;
          break;
        case TERROR:
          generos[TERROR] = generos[TERROR] + 1;
          break;
        case ACAO:
          generos[ACAO] = generos[ACAO] + 1;
          break;
        case ROMANCE:
          generos[ROMANCE] = generos[ROMANCE] + 1;
          break;
        }
        break;
      }
    }
  }

  //Fazer a contagem dos gêneros, verificando qual é o maior e se houve empate
  //Caso haja empate pegar quais filmes empataram
  for (i = 0; i < qtdGeneros; i++)
  {
    if (generos[i] == 0)
      continue;

    if (generos[i] > maiorGenero)
    {
      maiorGenero = generos[i];
      tipoGeneros[tamGeneros - 1] = (enum Generos)i;
    }

    else if (generos[i] == maiorGenero)
    {
      ++tamGeneros;
      tipoGeneros = (enum Generos *)realloc(tipoGeneros, sizeof(int) * tamGeneros);
      tipoGeneros[tamGeneros - 1] = (enum Generos)i;
    }
  }

  //Obter quantos filmes há no vetor de FILMES, dado um gênero específico
  for (i = 0; i < ctFilmes; i++)
    if (tipoGeneros[(tamGeneros - 1)] == filmes[i].genero)
      ctGenero++;

  if (maiorGenero >= ctGenero)
    return TODOS_FILMES_ASSISTIDOS;

  for (i = 0; i < tamGeneros; i++)
    imprimirFilmePorGenero(filmes, ctFilmes, tipoGeneros[i], historico,
                           posCliente, qtdFilmes, genero, classificacao);

  return OPERACAO_SUCEDIDA;
}