import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

public class FIFAFetchGUI {
    private static JTextField idField, ageField, nameField, countryField, clubField;
    private static JTextArea resultArea;
    private static JPanel resultPanel;
    private static FIFAFetch fifaFetch;
    private static List<FIFAPlayer> playersList;

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
        JLabel countryLabel = new JLabel("Country:");
        countryField = new JTextField();
        JLabel clubLabel = new JLabel("Club:");
        clubField = new JTextField();

        panel.add(idLabel);
        panel.add(idField);
        panel.add(ageLabel);
        panel.add(ageField);
        panel.add(nameLabel);
        panel.add(nameField);
        panel.add(countryLabel);
        panel.add(countryField);
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

        resultPanel = new JPanel();
        resultPanel.setLayout(new BoxLayout(resultPanel, BoxLayout.Y_AXIS));
        JScrollPane scrollPane = new JScrollPane(resultPanel);

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
        String country = countryField.getText();
        String club = clubField.getText();

        StringBuilder request = new StringBuilder("functionality:3");
        if (!id.isEmpty())
            request.append(",id:").append(id);
        if (!age.isEmpty())
            request.append(",age:").append(age);
        if (!name.isEmpty())
            request.append(",name:").append(name);
        if (!country.isEmpty())
            request.append(",country:").append(country);
        if (!club.isEmpty())
            request.append(",club:").append(club);

        List<FIFAPlayer> players = fifaFetch.getPlayers(request.toString());
        resultPanel.removeAll(); // limpa resultados anteriores

        if (players == null) {
            resultPanel.add(new JLabel("Jogador(es) não encontrado(s).")); // texto falando que nenhum jogador foi encontrado
        } else {
            playersList = players;
            for (FIFAPlayer player : playersList) {
                JButton playerButton = new JButton(player.name);
                playerButton.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        openPlayerWindow(player);
                    }
                });
                resultPanel.add(playerButton);
            }
        }

        resultPanel.revalidate();
        resultPanel.repaint();
    }

    private static void openPlayerWindow(FIFAPlayer player) {
        JFrame playerFrame = new JFrame(player.name);
        playerFrame.setSize(300, 200);
        playerFrame.setLayout(new GridLayout(3, 1));

        JLabel nameLabel = new JLabel("Jogador: " + player.name + "com id: " + player.id);
        playerFrame.add(nameLabel);

        JButton removeButton = new JButton("Remover Jogador");
        removeButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                StringBuilder remove_request = new StringBuilder("functionality:5");
                remove_request.append(",id:").append(player.id);
                Boolean result = fifaFetch.removePlayer(remove_request.toString());
                if (result){
                    System.out.println("remoção com sucesso");
                }else {
                    System.out.println("remoção falhou");
                }
                
                playerFrame.dispose();
            }
        });
        playerFrame.add(removeButton);

        JButton updateButton = new JButton("Update Player");
        updateButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // Implement update player functionality here
                JOptionPane.showMessageDialog(playerFrame, "Player " + player.name + " updated.");
                playerFrame.dispose();
            }
        });
        playerFrame.add(updateButton);
        playerFrame.setVisible(true);
    }

    public static void updatePlayerInList(int id, FIFAPlayer newInfo) {
        if (playersList == null) {
            return;
        }

        FIFAPlayer.updatePlayerInList(playersList, id, newInfo); //atualiza o cara na lista
        
        StringBuilder request = new StringBuilder("functionality:7");
      
        request.append(",id:").append(id); //id do cara não muda
        request.append(",age:").append(newInfo.age); //coloca os campos 
        request.append(",name:").append(newInfo.name);
        request.append(",country:").append(newInfo.country);
        request.append(",club:").append(newInfo.club);
        
    }
}
