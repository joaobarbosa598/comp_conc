class LEMonitor {

	private int leit;

    Buffer buffer = new Buffer("Inicio");
	public LEMonitor() { 
		this.leit = 0; 
	}

	// Entrada para leitores
	public synchronized void EntraLeitor() { 
		this.leit++; 
	}
	// Saida para leitores
	public synchronized void SaiLeitor() {
		this.leit--;
		if (this.leit == 0)
			notifyAll();
	}
	// Metodo para escritores
	public synchronized void Escrita(String str) {
		try {
			while (this.leit>0) { 
				wait(); 
			}
		} catch(InterruptedException e) {}

		//realiza a escrita de ’str’
		this.buffer.set(str);
		notify();
	}
}

class Leitor extends Thread
{
    LEMonitor leitor = new LEMonitor();
    public Leitor()
    {
    }

    public void run()
    {
        try
        {
            for(;;)
            {
                leitorescritor.EntraLeitor();
                this.leitor.buffer.get();
                leitorescritor.SaiLeitor();
            }
        } catch (Exception e) { }
    }
}

class Escritor extends Thread
{
    LEMonitor escritor = new LEMonitor();
    public Escritor()
    {
    }

    public void run()
    {
        try
        {
            for(int i=0;;i++)
            {
                escritor.Escrita("Estou escrevendo "+i);
            }
        } catch (Exception e) { }
    }
}

class Buffer
{
    String buffer = new String();
    public Buffer(String mensagem)
    {
        this.buffer = mensagem;
    }
    
    public String get()
    {
        return this.buffer;
    }

    public void set(String mensagem)
    {
        this.buffer = mensagem;
    }

}

class questao4{

	static final int L = 2;
    static final int E = 2;

    public static void main (String[] args)
    {
        Leitor[] leitor = new Leitor[L];
        Escritor[] escritor = new Escritor[E];
        int i;

        for(i = 0; i < L; i++)
        {
            leitor[i] = new Leitor();
            leitor[i].start();
        }

        for(i = 0; i < E; i++)
        {
            escritor[i] = new Escritor();
            escritor[i].start();
        }
    }

}