#include <cstdio>
#include <climits>
#include <string>
#include <vector>

using std::string;
using std::vector;

class AulaHeap
{
public:
  AulaHeap();
  AulaHeap(int n, int dados[]);
  ~AulaHeap();
  void escreve_niveis();
  void escreve(const string& prefixo = "", int i = 0);
  void insere(int p);
  int consulta_maxima();
  int extrai_maxima();
  void altera_prioridade(int i, int p);

  
private:
  vector<int> S;

  int pai(int i);
  int esquerdo(int i);
  int direito(int i);
  void troca(int i, int j);
  void desce(int i);
  void sobe(int i);
};


int main(void)
{
  AulaHeap h; // construtor AulaHeap()
  
  for (int i = 1; i <= 10; i++)
    h.insere(i);
  printf("h:\n");
  h.escreve();

  h.extrai_maxima();
  h.altera_prioridade(0, -3);
  printf("h:\n");
  h.escreve();

  return 0;

  int v[] = {1,2,3,4,5};
  
  AulaHeap h2(5, v); // construtor AulaHeap(int n, int dados[])
  h2.insere(15);
  printf("h2:\n");
  h2.escreve();

  AulaHeap h3(h2); // construtor de cópia padrão
  h2.insere(30);
  printf("h3:\n");
  h3.escreve();

  AulaHeap h4 = h2; // construtor de cópia padrão
  h2.insere(40);
  printf("h4:\n");
  h4.escreve();
  
  h = h2; // operador de atribuição, sem constructor ou constructor de cópia
  h.insere(100);
  printf("h2:\n");
  h2.escreve();
  printf("h:\n");
  h.escreve();

  h = AulaHeap(5, v); // construtor AulaHeap(int n, int dados[]), seguido de atribuição (de cópia, não transferência (move))
  printf("h:\n");
  h.escreve();

  h.extrai_maxima();
  h.altera_prioridade(0, -2);
  printf("h:\n");
  h.escreve();

  return 0;
}

/*

AulaHeap h2(5, v);  // o construtor é usado para construir h2
AulaHeap h3(h2);    // o construtor de cópia é usado para construir h3
AulaHeap h4 = h3;   // o construtor de cópia é usado para inicializar um objeto quando há uma atribuição SIMULTÂNEA com a declaração
h = h2;         // o operador de atribuição (cópia) é utilizado (não algum construtor ou transferência)
h = AulaHeap(5, v); // o construtor é usado para construir uma variável temporária, e depois o operação de atribuição (cópia?) é utilizado

** Quando são feitas cópias de objetos **

O C++ chama um construtor de cópia para fazer uma cópia de um objeto
conforme descrito nos casos acima. Se não houver um construtor de
cópias definido para a classe, o C ++ definirá um construtor de cópias
padrão, que copia cada campo (utilizando o construtor de cópia de cada
campo, se for o caso).


** Não escreva um construtor de cópias a menos que necessário **

Se o objeto não tiver ponteiros para a memória alocada dinâmicamente,
uma cópia "superficial" provavelmente será suficiente. Portanto, o
construtor de cópia padrão, o operador de atribuição padrão e o
destruidor padrão estão ok e você não precisa escrever seus próprios.


** Se você precisar de um construtor de cópias, também precisará de um **
** destruidor e um operador de atribuição (operator=) **

Se você precisa de um construtor de cópias, é porque precisa de algo
como uma cópia profunda ou algum outro gerenciamento de
recursos. Assim, é quase certo que você precisará definir um destruidor e
um operador de atribuição.


** Diferença entre construtor de cópia e atribuição **

Um construtor de cópia é usado para inicializar uma variável EM SUA
DECLARAÇÃO a partir de uma variável existente. É parecido com a
atribuição, mas mais simples:

1. Não há necessidade de testar para ver se ele está sendo inicializado por si mesmo
2. Não há necessidade de limpar (por exemplo, excluir) um valor existente (não há nenhum)
3. Uma referência a si mesma não é devolvida.


** Transferência **

Na maioria das vezes, o construtor de transferência ou o operador de
atribuição de trasnferência não são fornecidos pelo compilador. Se
forem, eles farão o mesmo que os construtores/operadores de cópia.

Regra: se você deseja um construtor de transferência e uma atribuição
de transferência que realmente realize a transferência de dados, você
terá que escrevê-los

*/


//*************************************
//*** IMPLEMENTAÇÕES DA CLASSE HEAP ***
//*************************************

AulaHeap::AulaHeap() {
}

AulaHeap::AulaHeap(int n, int dados[]) :
  S(dados, dados + n) {
  //TODO: implementar (constroi_max_heap)

}

AulaHeap::~AulaHeap() {
}

void AulaHeap::escreve_niveis() {
  int escritos = 0, fim_nivel = 1;

  for(auto const& elemento: S) {
  // Equivalente a for (unsigned i = 0; i < S.size(); i++) { printf("%d ", S[i]);
    printf("%d ", elemento);
    if (++escritos == fim_nivel) {
      putchar('\n');
      fim_nivel *= 2;
      escritos = 0;
    }
  }
  putchar('\n');
}

void AulaHeap::escreve(const string& prefixo, int i) {
  if (i < (int) S.size()) {
    bool ehEsquerdo = i % 2 != 0;
    bool temIrmao = i < (int) S.size()-1;
    
    printf(prefixo.c_str());
    printf(ehEsquerdo and temIrmao ? "├──" : "└──" );

    printf("%d\n", S[i]);
      
    escreve(prefixo + (ehEsquerdo ? "│   " : "    "), esquerdo(i));
    escreve(prefixo + (ehEsquerdo ? "│   " : "    "), direito(i));
  }
}

int AulaHeap::pai(int i) {
  return (i - 1) / 2;
}

int AulaHeap::esquerdo(int i) {
  return 2 * (i + 1) - 1;
}

int AulaHeap::direito(int i) {
  return 2 * (i + 1);
}

void AulaHeap::troca(int i, int j) {
  int aux = S[i];
  S[i] = S[j];
  S[j] = aux;
}

void AulaHeap::desce(int i) {
  //TODO: implementar

}

void AulaHeap::sobe(int i) {
  while (S[pai(i)] < S[i]) {
    troca(i, pai(i));
    i = pai(i);
  }
}

void AulaHeap::insere(int p) {
  S.push_back(p);
  sobe(S.size()-1);
}

int AulaHeap::consulta_maxima() {
  //TODO: implementar

}

int AulaHeap::extrai_maxima() {
  //TODO: implementar

}

void AulaHeap::altera_prioridade(int i, int p) {
  //TODO: implementar

}