class Buffer {
	static final int N = 1; //qtde de elementos no buffer
	private int[] buffer = new int[N]; //area de dados compartilhada
	//variaveis de estado
	private int count=0; //qtde de posicoes ocupadas no buffer
	private int in=0; //proxima posicao de insercao
	private int out=0; //proxima posicao de retirada

// Construtor
	public Buffer() {
		// inicializa o buffer 
		for(int i=0; i<N; i++){
			buffer[i] = 0;
		}
	}

	// Insere um item
	public synchronized void Insere (int item) throws InterruptedException {
		try {
			while (count==N) {
				wait();
			}
			buffer[in%N] = item;
			in++;
			count++;
			System.out.print("Inseri "+ item+ ", tenho o buffer: ");
			exibeBuffer();
			notify();
		} catch (InterruptedException e) {}
	}

	// Remove um item
	public synchronized int Remove (int id) throws InterruptedException {
		
		try {
			int aux;
			while (count==0) {
				wait();
			}
			
			aux = this.buffer[out];
			this.buffer[out] = 0;
			out = (out+1)%N;
			count--;
			System.out.print("Removi "+ aux+ ", tenho o buffer: ");
			exibeBuffer();
			notify();
			return aux;
			} catch(InterruptedException e) { return -1; }
	}

	public void exibeBuffer(){
		for(int i = 0; i < N; i++){
			System.out.print(buffer[i]+" ");
		}
		System.out.println();
	}
}

//Consumidor
class Consumidor extends Thread {
	int id;
	int delay;
	Buffer buffer;
	
	// Construtor
	Consumidor (int id, int delayTime, Buffer b) {
	 this.id = id;
	 this.delay = delayTime;
	 this.buffer = b;
	}
	
	// Método executado pela thread
	public void run () {
	 int item;
	 try {
	   for (;;) {
	     item = this.buffer.Remove(id);
	     sleep(this.delay); //...simula o tempo para fazer algo com o item retirado
	   }
	 } catch (InterruptedException e) { return; }
	}
}

//Produtor
class Produtor extends Thread {
	int id;
	int delay;
	Buffer buffer;
	
	// Construtor
	Produtor (int id, int delayTime, Buffer b) {
	 this.id = id;
	 this.delay = delayTime;
	 this.buffer = b;
	}
	
	// Método executado pelo thread
	public void run () {
	 try {
	   for (;;) {
	     this.buffer.Insere(this.id); //simplificacao: insere o proprio ID
	     sleep(this.delay);
	   }
	 } catch (InterruptedException e) { return; }
	}
}


class questao2{
	static final int P = 2;
	static final int C = 1;

	public static void main(String[] args){
		int i;
	    Buffer buffer = new Buffer();      // Monitor
	    Consumidor[] cons = new Consumidor[C];   // Consumidores
	    Produtor[] prod = new Produtor[P];       // Produtores
	    

	    for (i=0; i<P/2; i++) {
	       prod[i] = new Produtor(i+1, 1000, buffer);
	       prod[i].start(); 
	    }

	    for (i=P/2; i<P; i++) {
	       prod[i] = new Produtor(i+1, 1000, buffer);
	       prod[i].start(); 
	    }
	    for (i=0; i<C/2; i++) {
	       cons[i] = new Consumidor(i+1, 1000, buffer);
	       cons[i].start(); 
	    }

	    for (i=C/2; i<C; i++) {
	       cons[i] = new Consumidor(i+1, 1000, buffer);
	       cons[i].start(); 
	    }

	  }
}