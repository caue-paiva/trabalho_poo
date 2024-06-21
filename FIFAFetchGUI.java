import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class FIFAFetchGUI {
    private static JTextField idField, ageField, nameField, nationalityField, clubField;
    private static JTextArea resultArea;
    private static FIFAFetch fifaFetch;

    public static void createAndShowGUI() {
        JFrame frame = new JFrame("FIFA Player Fetch");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(600, 400);

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(7, 2));

        JLabel idLabel = new JLabel("ID:");
        idField = new JTextField();
        JLabel ageLabel = new JLabel("Age:");
        ageField = new JTextField();
        JLabel nameLabel = new JLabel("Name:");
        nameField = new JTextField();
        JLabel nationalityLabel = new JLabel("Nationality:");
        nationalityField = new JTextField();
        JLabel clubLabel = new JLabel("Club:");
        clubField = new JTextField();

        panel.add(idLabel);
        panel.add(idField);
        panel.add(ageLabel);
        panel.add(ageField);
        panel.add(nameLabel);
        panel.add(nameField);
        panel.add(nationalityLabel);
        panel.add(nationalityField);
        panel.add(clubLabel);
        panel.add(clubField);

        JButton searchButton = new JButton("Search");
        searchButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                searchPlayer();
            }
        });

        panel.add(new JLabel());
        panel.add(searchButton);

        resultArea = new JTextArea();
        resultArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(resultArea);

        frame.getContentPane().add(panel, BorderLayout.NORTH);
        frame.getContentPane().add(scrollPane, BorderLayout.CENTER);

        frame.setVisible(true);

        // Setup connection
        String serverAddress = "127.0.0.1";
        int serverPort = 9090;
        fifaFetch = new FIFAFetch(serverAddress, serverPort);
    }

    private static void searchPlayer() {
        String id = idField.getText();
        String age = ageField.getText();
        String name = nameField.getText();
        String nationality = nationalityField.getText();
        String club = clubField.getText();

        StringBuilder request = new StringBuilder("functionality:3");
        if (!id.isEmpty())
            request.append(",id:").append(id);
        if (!age.isEmpty())
            request.append(",age:").append(age);
        if (!name.isEmpty())
            request.append(",name:").append(name);
        if (!nationality.isEmpty())
            request.append(",nationality:").append(nationality);
        if (!club.isEmpty())
            request.append(",club:").append(club);

        String response = fifaFetch.sendRequest(request.toString());
        System.out.println(response);
        resultArea.setText(response);
    }
}
