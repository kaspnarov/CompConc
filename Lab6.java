/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 6 */
/* Nome: Thiago Campbell (118060285)*/
/* -------------------------------------------------------------------*/

//classe da estrutura de dados (recurso) compartilhado entre as threads
class S {
   //recurso compartilhado
   private int tamVetor;
   private int[] vetor;

   //construtor
   public S() {
      this.tamVetor = 10;
      this.vetor = new int[this.tamVetor];
      for(int i = 0; i < this.tamVetor; i++){
         this.vetor[i] = 0; 
      }    
   }

   //operacao de escrita sobre o recurso compartilhado
   public synchronized void inc(int i) { 
      this.vetor[i] += 1; 
   }
   //operacao de leitura sobre o recurso compartilhado
   public synchronized int get(int i) { 
      return this.vetor[i]; 
   }
   //leitura do tamanho do vetor
   public int getTamVetor(){
      return this.tamVetor;
   }
}

//classe que estende Thread e implementa a tarefa de cada thread do programa 
class T extends Thread {
   //identificador da thread
   private int id;
   //objeto compartilhado com outras threads
   S s;
  
   //construtor
   public T(int tid, S s) { 
      this.id = tid; 
      this.s = s;
   }

   //metodo main da thread
   public void run() {
      System.out.println("Thread " + this.id + " iniciou!");
      for (int j = this.id; j < this.s.getTamVetor(); j += TIncrementoVetor.N){
         this.s.inc(j);
      } 
      System.out.println("Thread " + this.id + " terminou!"); 
   }
}

//classe da aplicacao
class TIncrementoVetor {
   static final int N = 2;

   public static void main (String[] args) {
      //reserva espaço para um vetor de threads
      Thread[] threads = new Thread[N];

      //cria uma instancia do recurso compartilhado entre as threads
      S s = new S();

      //cria as threads da aplicacao
      for (int i=0; i<threads.length; i++) {
         threads[i] = new T(i, s);
      }

      //inicia as threads
      for (int i=0; i<threads.length; i++) {
         threads[i].start();
      }

      //espera pelo termino de todas as threads
      for (int i=0; i<threads.length; i++) {
         try { threads[i].join(); } catch (InterruptedException e) { return; }
      }

      //imprime os valores do vetor para checar corretude do programa
      for (int i = 0; i < s.getTamVetor(); i++){
         System.out.println("Valor de vetor[" + i + "] = " + s.get(i)); 
      }
   }
}
