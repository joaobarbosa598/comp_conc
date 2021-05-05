/*Nome: João Vitor de Freitas Barbosa
DRE: 117055449
Atividade 5 do laboratório 6*/
class Vetor{
	//Vetor da classe
	private int[] vetor;

	private int tamanhoVetor;

	//construtor
	public Vetor(int tamanhoVetor){
		this.vetor = new int[tamanhoVetor];
		this.tamanhoVetor = tamanhoVetor;
	}

	//funcao que incrementa de 1 o elemento do vetor na posicao i
	public synchronized void incrementaElemento(int i){
		this.vetor[i] += 1;
	}

	public int getTamanhoVetor(){
		return this.tamanhoVetor;
	}

	public void print(){
		for(int elemento: this.vetor){
			System.out.printf("%d ", elemento);
		}

		System.out.printf("\n\n");
	}
}

class myThreads extends Thread{
	//identificador da thread
	private int id;
	private int nThreads;
	//objeto compartilhado entre as threads
	Vetor vetor;

	//construtor
	public myThreads(int tid, int nThreads, Vetor vetor){
		this.id = tid;
		this.nThreads = nThreads;
		this.vetor = vetor;
	}

	//metood main da thread
	public void run(){
		System.out.println("Thread "+ this.id + " iniciou!");
		for(int i = id; i < vetor.getTamanhoVetor(); i+=nThreads){
			this.vetor.incrementaElemento(i);
		}
		System.out.println("Thread "+ this.id + " terminou!");
	}
}

//classe da minha aplicacao
class atividade5{
	static final int N = 4;

	public static void main (String[] args){
		//reserva espaço para um vetor de threads
		Thread[] threads = new Thread[N];

		//cria uma instancia do recurso compartilhado entre as threads
		Vetor vetor = new Vetor(N*10);

		System.out.printf("Meu vetor inicial: ");  vetor.print();
		//cria as threads da aplicacao
		for(int i = 0; i < threads.length; i++){
			threads[i] = new myThreads(i, N, vetor);
		}

		//incia as threads
		for(int i = 0; i < threads.length; i++){
			threads[i].start();
		}

		//espera pelo termino de todas as threads
		for(int i = 0; i<threads.length; i++){
			try { threads[i].join(); } catch (InterruptedException e) { return; }
		}

		System.out.printf("Meu vetor resultante: ");  vetor.print();
	}
}
