package sgbd.stockage;

public interface Nuplet {

	/**
	 * 
	 * @param i position de l'attribut
	 * @return valeur de l'attribut
	 */
	public Object getAtt(int i); 
	
	public void putAtt(int i, Object o);
	
	public int size();
	
	public String toString();
}
