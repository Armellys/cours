package sgbd.stockage;

public interface Table {
	
	public Nuplet get(int pos);
	public Nuplet[] fullScan();
	public Nuplet[] getByAtt(int att, Object value); // Récupère les nuplets qui ont la valeur value sur l'attribut nr att
	public int size();
	public void put(Nuplet n);
	public void insert(Nuplet n);
	public void delete(Nuplet n, int att, Object value); // efface tous les Nuplets dont att est égal à value
	public void update(Nuplet n, int att, Object oldValue, Object newValue); // modifie tous les Nuplets dont att était égal à oldValue et leur met la valeur newValue

}
