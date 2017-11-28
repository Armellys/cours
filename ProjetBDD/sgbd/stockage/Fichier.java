package sgbd.stockage;

public interface Fichier {

	public void store(int pos, Object o);
	
	public Object get(int pos);
	
	public long getCurrentSize();
	
}
