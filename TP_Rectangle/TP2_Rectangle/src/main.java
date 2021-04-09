import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;
import java.util.Timer;

class Point {
	public int x;
	public int y;

	public Point(int x, int y) {
		this.x = x;
		this.y = y;
	}
}

public class main {

	public static Point[] Points;
	public static int n, h, l;
	public static int test;

	public static void main(String args[]) throws IOException {
		
		getDataFromFile("Verif/ex_N100000_res100000");// to get datas from the files in Verif folder
		//getDataFromUser(); // to get the datas from user like on http://contest.fil.univ-lille1.fr/

		//long startTime = System.currentTimeMillis();

		 //int result = findSurfaceN3(Points, h);  // algo O(n^3)
		 //int result = findSurfaceN2(Points, h);  // algo O(n^2)
		 int result = findSurfaceDivideForConquer(Points, 0);

		//long estimatedTime = System.currentTimeMillis() - startTime;
     	//System.out.println("elapsedTime : " + estimatedTime);
		
		System.out.print(result);
	}

	public static void getDataFromUser() {

		Scanner clavier = new Scanner(System.in);
		l = clavier.nextInt();
		h = clavier.nextInt();
		n = clavier.nextInt();

		Points = new Point[n + 2];
		Points[0] = new Point(0, 0);
		Points[n + 1] = new Point(l, 0);
		int tempo = -1;
		int j = 0;
		for (int i = 0; i < 2 * n; i++) {

			if (i % 2 != 0 && tempo > -1) {
				Points[j] = new Point(tempo, clavier.nextInt());
			} else {
				j++;
				tempo = clavier.nextInt();
			}
		}
	}

	public static void getDataFromFile(String fileName) throws IOException {
		
		File source = new File(fileName);
		BufferedReader buffer = null;
		buffer = new BufferedReader(new FileReader(source));
		String stringPoint;
		
		try {
			String[] lAndH = buffer.readLine().split(" ");
			l = Integer.parseInt(lAndH[0]);
			h = Integer.parseInt(lAndH[1]);
			n = Integer.parseInt(buffer.readLine()) + 2;
			Points = new Point[n];
			Points[0] = new Point(0, 0);
			int i = 1;
			while ((stringPoint = buffer.readLine()) != null) {
				String[] point = stringPoint.split(" ");
				Points[i] = new Point(Integer.parseInt(point[0]), Integer.parseInt(point[1]));
				i++;
			}
			Points[i] = new Point(l, 0);
		} catch (FileNotFoundException e) {
			throw new IOException(e);
		} finally {
			buffer.close();
		}
	}

	public static int findSurfaceN3(Point t[], int h) { // algo O(n^3)
		int result = 0;
		// on parcours  l'ensemble des pair de points qu'on peut avoir
		for (int i = 0; i < t.length; i++) {
			
			int yMin = h;
			int firstX = t[i].x;
			int currY = 0;

			 for (int j = i + 1; j < t.length; j++) {
				for (int k = i + 1; k < j; k++) { // retrouver l'odonné minimale entre les deux points t[i] et t[j]
					currY = t[k].y;

					if (currY < yMin && j - i > 1) {
						yMin = currY;

					}

				}
				int length = t[j].x - firstX;
				int tempoResult = yMin * length;
				if (tempoResult > result) {
					result = tempoResult;
				}
			}
		}
		return result;
	}

	public static int findSurfaceN2(Point t[], int h) { // algo O(n^2)
		int result = 0;
		for (int i = 0; i < t.length; i++) {
			int yMin = h;
			int firstX = t[i].x;
			int currY = 0;

			for (int j = i + 1; j < t.length; j++) {
				currY = t[j - 1].y;

				if (currY < yMin && j - i > 1) {
					yMin = currY;
				}

				int length = t[j].x - firstX;
				int tempoResult = yMin * length;
				if (tempoResult > result) {
					result = tempoResult;
				}
			}
		}
		return result;
	}

	public static int findSurfaceDivideForConquer(Point t[], int result) { // algo divideForConquer
		int realResult = result;
		int minY = h;
		int intervalI = 0;
		
		for (int i = 1; i < t.length - 1; i++) { // trouve le point du rectangle passé en paramètre ou y est le minimum
			if (t[i].y <= minY) {
				minY = t[i].y;
				intervalI = i;
			}
		}

		int tempoResult = 0;
		if (minY == h) { // condition d'arrêt si on ymin est égale à la plus grande hauteur, on calcule l'air
			tempoResult = (t[t.length - 1].x - t[0].x) * minY;
			if (tempoResult > realResult) {
				realResult = tempoResult;
			}
			return realResult;
		}

		Point g[] = Arrays.copyOfRange(t, 0, intervalI + 1);
		Point d[] = Arrays.copyOfRange(t, intervalI, t.length);

		tempoResult = (d[d.length - 1].x - g[0].x) * minY; //distance entre le point le plus à gauche du 1er sous rectangle et le point le plus à droite du 2ième sous rectangle
		if (tempoResult > realResult) {
			realResult = tempoResult;
		}

		int gResult = findSurfaceDivideForConquer(g, realResult);
		int dResult = findSurfaceDivideForConquer(d, realResult);

		if (gResult > realResult) {
			realResult = gResult;
		}

		if (dResult > realResult) {
			realResult = dResult;
		}
		return realResult;
	}
}
// je passe 9 tests sur 10, n^2 passe de 1 à 8 avec le 11 et le divideandconquer passe 1 à 9