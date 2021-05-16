class LEMonitor {

	private int leit;
	public int passeiEscrita;
	private int passeiEntraLeitura;

	public LEMonitor() { 
		this.leit = 0; 
	}

	// Entrada para leitores
	public synchronized void EntraLeitor(int id) { 
		this.leit++; 
	}
	// Saida para leitores
	public synchronized void SaiLeitor(int id) {
		System.out.println("Leitor "+id+ " terminou de ler com Leitores: "+this.leit);
		this.leit--;
		if (this.leit == 0) {
			System.out.println("Leitor "+id+ " vai notificar todo mundo");
			notifyAll();
		}
	}
	// Metodo para escritores
	public synchronized void Escrita(int id) {
		System.out.println("Escritor "+id+ " comecou a escrever");
		try {
			while (this.leit>0) { 
				System.out.println("Escritor "+id+ " vai se bloquearrrrrrrrrrrrrrrrrrr com Leitores: "+this.leit);
				wait();
				System.out.println("Escritor "+id+ " passou pela verificacao do wait de escrever"); 
			}
		} catch(InterruptedException e) {}
		this.passeiEscrita=1;
		//realiza a escrita de ’str’
		System.out.println("Escritor "+id+ " terminou de escrever");
	}
}

class Leitor extends Thread
{
    LEMonitor leitor;
    int id;
    public Leitor(int id, LEMonitor leitor)
    {
    	this.id = id;
    	this.leitor = leitor;
    }

    public void run()
    {
        try
        {
            for(;;)
            {
            	this.leitor.EntraLeitor(this.id);
        		System.out.println("Leitor "+id+ " está lendo");
                this.leitor.SaiLeitor(this.id);
                sleep(1000);
            }
        } catch (Exception e) { }
    }
}

class Escritor extends Thread
{
    LEMonitor escritor;
    int id;
    public Escritor(int id, LEMonitor escritor)
    {
    	this.id = id;
    	this.escritor = escritor;
    }

    public void run()
    {
        try
        {
            for(;;)
            {
                this.escritor.Escrita(this.id);
                sleep(1000);
            }
        } catch (Exception e) { }
    }
}

class questao4{
	static final int L = 10;
    static final int E = 10;

    public static void main (String[] args)
    {
        Leitor[] leitor = new Leitor[L];
        Escritor[] escritor = new Escritor[E];
        //cria uma instancia do recurso compartilhado entre as threads
        LEMonitor monitor = new LEMonitor();
        int i;
        
        for(i = 0; i < L/2; i++)
        {
            leitor[i] = new Leitor(i, monitor);
            leitor[i].start();
        }
        for(i = 0; i < E/2; i++)
        {
            escritor[i] = new Escritor(i, monitor);
            escritor[i].start();
        }
        for(i = L/2; i < L; i++)
        {
            leitor[i] = new Leitor(i, monitor);
            leitor[i].start();
        }

        for(i = E/2; i < E; i++)
        {
            escritor[i] = new Escritor(i, monitor);
            escritor[i].start();
        }
    }
}