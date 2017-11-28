package sgbd.impl;

import sgbd.stockage.Fichier;
import java.io.*;

public class FichierInt implements Fichier{

	private String fp;
	private RandomAccessFile f;
	private int nupletSize;
	private int currentLength;
	
	public FichierInt(String filePath, int nupletSize){
		this.fp = filePath;
		this.nupletSize = nupletSize;
	}

	public void store(int pos, Object o) {
		try {
			this.f = new RandomAccessFile(this.fp, "rw");
			NupletInt n = (NupletInt) o;
			byte[] b = new byte[nupletSize];
			for(int i=0;i<nupletSize;i++)
				b[i] = (byte)((NupletInt) o).getAtt(i);
			this.f.seek(pos*nupletSize);
			this.f.write(b);
			this.f.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public Object get(int pos) {
		try {
			this.f = new RandomAccessFile(this.fp, "rw");
			byte[] b = new byte[nupletSize];
			this.f.seek(pos*nupletSize);
			f.read(b);
			f.close();
			return new NupletInt(b);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	@Override
	public long getCurrentSize() {
		return this.currentLength;
	}
	
	

}
