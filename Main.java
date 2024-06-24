public class Main {
    /*
    Classe main para rodar a GUI em java 
    */
    public static void main(String[] args) {
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                FIFAFetchGUI.createAndShowGUI();
            }
        });
    }
}
