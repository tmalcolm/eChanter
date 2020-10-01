import java.util.InputMismatchException;
import java.util.Scanner;



public class ToneGen {

    Scanner in = new Scanner(System.in);


	int sampleRate = 0;
	int freq = 0;
	int amplitude = 0;
	int dcOffset = 0;
	String another = "";

    double p, a;
    double partials [][] = {
                       {1.0,  1.0 },
                       {3.0,  0.75},
                       {5.0,  0.5 },
                       {7.0,  0.14},
                       {9.0,  0.5 },
                       {11.0, 0.12},
                       {13.0, 0.17}
				   };


	public ToneGen() {
	}


    public void start() {
		boolean loop = true;

		while (loop) {
			sampleRate = 0;
			freq = 0;
			amplitude = -1;
			dcOffset = -1;

			while (sampleRate == 0) {
			   try {
				   System.out.println("Sample rate (Hz):");
				   sampleRate=in.nextInt();
				} catch (InputMismatchException ex) {
				} catch (Throwable ex) {
					System.exit(-1);
				}
			}

			while (freq == 0) {
			   try {
				   System.out.println("Frequency (Hz):");
				   freq=in.nextInt();
				} catch (InputMismatchException ex) {
				} catch (Throwable ex) {
					System.exit(-1);
				}
			}

			while (amplitude == -1) {
			   try {
				   System.out.println("Amplitude scaling (1-127):");
				   amplitude=in.nextInt();
				} catch (InputMismatchException ex) {
				} catch (Throwable ex) {
					System.exit(-1);
				}
			}
			while (dcOffset == -1) {
			   try {
				   System.out.println("DC Offset (0-127):");
				   dcOffset=in.nextInt();
				} catch (InputMismatchException ex) {
				} catch (Throwable ex) {
					System.exit(-1);
				}
			}

			System.out.println("Generating Sinewave .....");
			System.out.println("Samplerate: "+sampleRate+"Hz");
			System.out.println("Frequency:  "+freq+"Hz");
			System.out.println("Amplitude correction: "+amplitude);
			System.out.println("Number of Samples: "+(int)(sampleRate/freq));


			//generate();
//			generateInstrument();
			generateInstrumentWORKING();

			System.out.println("");
			System.out.println("Ctrl-C to quit");
			System.out.println("");




/*
			System.out.println("Quit (Y/N)?");
       		another = in.nextLine();
       		another = another.trim();
            if (another.length() > 0) {
				another.toUpperCase();
				if (another.startsWith("Y")) {
					loop = false;
					break;
				}
			}
*/

		}


	}




	// Sinewave creation algorythm comes from:
	// http://embeddedsystemdesign.blogspot.com/2007/12/digital-dual-tone-generation-using.html
	//
	public void generateInstrument() {

        double increment = 0;
        int nSamples = 0;
		int i, n, s = 0;
		double v = 0.0;

		nSamples = sampleRate/freq;
		increment = 2.0 * Math.PI * ((float)freq/(float)sampleRate);

		System.out.print("f"+freq+"_sr"+sampleRate+"_len = "+nSamples+";");
		System.out.print("f"+freq+"_sr"+sampleRate+" ["+nSamples+"] = { ");

        for(i = 0; i < nSamples; i++) {

            s = 0;


      for (int j = 0; j < partials.length; j++) {

        p = partials[j][0];
        a = partials[j][1];

        v += Math.sin(p*increment*i) * a;

      }


            s = dcOffset + (int)(amplitude * v);

			System.out.print(s);

			if (i == nSamples -1) {
				System.out.println(" };");
			} else if (i % 20 == 0) {
				System.out.println(",");
			} else {
				System.out.print(", ");
			}

		}

	}



	// Sinewave creation algorythm comes from:
	// http://embeddedsystemdesign.blogspot.com/2007/12/digital-dual-tone-generation-using.html
	//
	public void generateInstrumentWORKING() {

        double increment = 0;
        int nSamples = 0;
		int i, n, s = 0;

		nSamples = sampleRate/freq;
		increment = 2.0 * Math.PI * ((float)freq/(float)sampleRate);

		System.out.print("f"+freq+"_sr"+sampleRate+"_len = "+nSamples+";");
		System.out.print("f"+freq+"_sr"+sampleRate+" ["+nSamples+"] = { ");

        for(i = 0; i < nSamples; i++) {

            s = dcOffset + (int)(amplitude * (

			Math.sin(1*increment*i) *1.0
			+ Math.sin(3*increment*i) *0.75
			+ Math.sin(5*increment*i) *0.5
			+ Math.sin(7*increment*i) *0.14
			+ Math.sin(9*increment*i) * 0.5
			+ Math.sin(11*increment*i) * 0.12
			+ Math.sin(13*increment*i) * 0.17
            )
            );

			System.out.print(s);

			if (i == nSamples -1) {
				System.out.println(" };");
			} else if (i % 20 == 0) {
				System.out.println(",");
			} else {
				System.out.print(", ");
			}

		}

	}


	// Sinewave creation algorythm comes from:
	// http://embeddedsystemdesign.blogspot.com/2007/12/digital-dual-tone-generation-using.html
	//
	public void generateSine() {

		double wT, y0, y1, y2 = 0;
		int nSamples = 0;
		int y = 0;

		nSamples = sampleRate/freq;
		wT = 2.0 * Math.PI * ((float)freq/(float)sampleRate);

		y0 = 0; /*initial value*/
		y1 = Math.sin (wT); /*initial value: sin (wT)*/

		System.out.print("f"+freq+"_sr"+sampleRate+"_len = "+nSamples+";");
		System.out.print("f"+freq+"_sr"+sampleRate+" ["+nSamples+"] = { ");

		for (int i=0; i<nSamples; i++) {

			y2 = (2*y1*Math.cos (wT)-y0); /*compute y2*/
			y = (int)(y2*amplitude)+50; /*amplitude scaling and dc shift*/
			y0 = y1; /*assign present values to previous value*/
			y1 = y2;
			System.out.print(y);
			if (i == nSamples -1) {
				System.out.println(" };");
			} else if (i % 20 == 0) {
				System.out.println(",");
			} else {
				System.out.print(", ");
			}

		}

	}


	public static void main(String[] args) {

		(new ToneGen()).start();

	}


}