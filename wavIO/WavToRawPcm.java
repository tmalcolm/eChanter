// GUI stuff
import java.awt.*;
import java.awt.event.*;
import java.io.*; // File input/output

import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import javax.sound.sampled.*;


public class WavToRawPcm extends JFrame implements ActionListener
{
	JButton openButton, saveButton, convertButton;
	JTextField txtInputFile, txtOutputFile;
	JLabel lblOutput;

	//Create a file chooser
	final JFileChooser fc = new JFileChooser();

	WavReader wavReader = null;


	// this is what the java app will execute when this program is started
	public static void main(String args[])
	{
		new WavToRawPcm();
	}

	// this is our constructor
	public WavToRawPcm()
	{
		setLayout(new GridLayout(0,1));


        JPanel inPanel = new JPanel();//new SpringLayout());


		txtInputFile = new JTextField("test.wav");
		txtInputFile.setBounds(0, 0, 200,20);
		inPanel.add(txtInputFile);

		openButton = new JButton("Input WAV file");
		openButton.setBounds(200, 0, 100, 20);
		openButton.addActionListener(this);
		inPanel.add(openButton);

		getContentPane().add(inPanel);

        JPanel outPanel = new JPanel();//new SpringLayout());

		txtOutputFile = new JTextField("test.h");
		txtOutputFile.setBounds(0, 0, 200,20);
		outPanel.add(txtOutputFile);

		saveButton = new JButton("Output PCM file");
		saveButton.setBounds(200, 0, 100, 20);
		saveButton.addActionListener(this);
		outPanel.add(saveButton);

		getContentPane().add(outPanel);



		// Format conversion choices





		// from Swing Tutorial
		//Create a text area.
        JTextArea textArea = new JTextArea("");
        textArea.setFont(new Font("Serif", Font.ITALIC, 16));
        textArea.setLineWrap(true);
        textArea.setWrapStyleWord(true);
        JScrollPane areaScrollPane = new JScrollPane(textArea);
        areaScrollPane.setVerticalScrollBarPolicy(
                        JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        areaScrollPane.setPreferredSize(new Dimension(250, 250));
        areaScrollPane.setBorder(
            BorderFactory.createCompoundBorder(
                BorderFactory.createCompoundBorder(
                                BorderFactory.createTitledBorder("Plain Text"),
                                BorderFactory.createEmptyBorder(5,5,5,5)),
                areaScrollPane.getBorder()));

		getContentPane().add(areaScrollPane);

		// show the application window
		setTitle("WavToRawPcm - based on WAV IO Test by Evan X. Merz - www.thisisnotalabel.com");
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setSize(300,300);
		setVisible(true);
	}



	public void actionPerformed(ActionEvent e) {
		//Handle open button action.
		if (e.getSource() == openButton) {
			int returnVal = fc.showOpenDialog(this);

			if (returnVal == JFileChooser.APPROVE_OPTION) {
				File file = fc.getSelectedFile();
				//This is where a real application would open the file.

				txtInputFile.setText(file.getName());

				if (wavReader != null) {
					wavReader.close();
				}
				wavReader = new WavReader(file);
				wavReader.displayConversionFormats(this);


			} else {


			}
	   } else if (e.getSource() == saveButton) {

	   } else if (e.getSource() == convertButton) {

	   }

	}


}


class WavReader {

	File audioFile = null;
    AudioInputStream audioInputStream = null;

	public WavReader(File file) {
		audioFile = file;
    }

	public void close() {

	}

	public void displayConversionFormats(JFrame gui) {

		if (audioInputStream == null) {
			try {
				AudioInputStream audioInputStream =
					AudioSystem.getAudioInputStream(audioFile);


				System.out.println(audioInputStream.getFormat().toString());


				AudioFormat outDataFormat = new AudioFormat((float) 8000.0,
					(int) 8, (int) 1, true, false);

				if (AudioSystem.isConversionSupported(outDataFormat,
				    							audioInputStream.getFormat())) {

					System.out.println("CAN CONVERT!!");
				}

				AudioFormat[] formats =
					AudioSystem.getTargetFormats(AudioFormat.Encoding.PCM_UNSIGNED,
													audioInputStream.getFormat());

				for (int i=0; i< formats.length; i++) {
					System.out.println("\t"+formats[i].toString());
				}

				System.out.println();

			} catch (Exception ex) {
				JOptionPane.showMessageDialog(gui,
			    "Could not open WAV file\\n" + audioFile.getName(),
			    "WAV Audio error",
			    JOptionPane.ERROR_MESSAGE);
			}
		}
	}

}

