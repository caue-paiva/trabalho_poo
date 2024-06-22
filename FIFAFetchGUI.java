import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

public class FIFAFetchGUI {
    private static JTextField idField, ageField, nameField, countryField, clubField;
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
            System.out.println(players);
            FIFAFetchGUI.playersList = players;
            FIFAFetchGUI.showPlayersButtons();
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
                FIFAFetchGUI.removePlayer(player.id, playerFrame);
            }
        });
        playerFrame.add(removeButton);

        JButton updateButton = new JButton("Update Player");
        updateButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                FIFAFetchGUI.updatePlayer(player, playerFrame);
                playerFrame.dispose();
            }
        });
        playerFrame.add(updateButton);
        playerFrame.setVisible(true);
    }

    //atualiza a GUI com a lista de botões dos jogadores contidos na lista da variável estática playersList
    private static void showPlayersButtons(){
        resultPanel.removeAll(); //remove os botões antigos
        for (FIFAPlayer player : FIFAFetchGUI.playersList) { //coloca os botões novos
            JButton playerButton = new JButton(player.name);
            playerButton.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    openPlayerWindow(player);
                }
            });
            resultPanel.add(playerButton);
        }
        resultPanel.revalidate(); //atualiza a GUI
        resultPanel.repaint();
    }

    private static void removePlayer(int id, JFrame playerFrame){
        StringBuilder remove_request = new StringBuilder("functionality:5");
        remove_request.append(",id:").append(id);
        Boolean result = fifaFetch.removePlayer(remove_request.toString());
        if (result) {
            JOptionPane.showMessageDialog(playerFrame, "Remoção com sucesso");
        } else {
            JOptionPane.showMessageDialog(playerFrame, "Remoção falhou");
        }
        playerFrame.dispose();

        Boolean resultado = FIFAFetchGUI.playersList.removeIf(player -> player.id == id); //remove o jogador da lista de jogadores se o id dele der match
        System.out.println(resultado);
        FIFAFetchGUI.showPlayersButtons(); //atualiza a GUI

    }

    private static void updatePlayer(FIFAPlayer player, JFrame playerFrame){
        JFrame updateFrame = new JFrame("Atualizar jogador");
        updateFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        updateFrame.setSize(400, 300);

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(5, 2)); // Adjust grid layout to fit all components

        // Name field
        JLabel nameLabel = new JLabel("Name:");
        JTextField nameField = new JTextField();
        nameField.setText(player.name); // Set default value
        panel.add(nameLabel);
        panel.add(nameField);

        // Age field
        JLabel ageLabel = new JLabel("Age:");
        JTextField ageField = new JTextField();
        ageField.setText(String.valueOf(player.age)); // Set default value
        panel.add(ageLabel);
        panel.add(ageField);

        // Country field
        JLabel countryLabel = new JLabel("Country:");
        JTextField countryField = new JTextField();
        countryField.setText(player.country); // Set default value
        panel.add(countryLabel);
        panel.add(countryField);

        // Club field
        JLabel clubLabel = new JLabel("Club:");
        JTextField clubField = new JTextField();
        clubField.setText(player.club); // Set default value
        panel.add(clubLabel);
        panel.add(clubField);

        // botão de submeter
        JButton submitButton = new JButton("Atualizar");
        submitButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String name = nameField.getText();
                String country = countryField.getText();
                String club = clubField.getText();
                String age = ageField.getText();

                StringBuilder update_request = new StringBuilder("functionality:7");
                update_request.append(",id:").append(player.id);
                if (!age.isEmpty())
                    update_request.append(",age:").append(age);
                if (!name.isEmpty())
                    update_request.append(",name:").append(name);
                if (!country.isEmpty())
                    update_request.append(",country:").append(country);
                if (!club.isEmpty())
                    update_request.append(",club:").append(club);

                Boolean result = fifaFetch.updatePlayer(update_request.toString());
                if (result) {
                    JOptionPane.showMessageDialog(playerFrame, "Atualização com sucesso");
                } else {
                    JOptionPane.showMessageDialog(playerFrame, "Atualização falhou");
                }
                FIFAFetchGUI.updatePlayerInList(player.id, name, country, club, Integer.parseInt(age));
                updateFrame.dispose();
            }
        });
        panel.add(new JLabel()); // Empty label for layout purposes
        panel.add(submitButton);
    
        updateFrame.add(panel);
        updateFrame.setVisible(true);
    }

    public static void updatePlayerInList(int id, String name, String country, String club, int age) {
        if (playersList == null) {
            return;
        }
        for (FIFAPlayer player : FIFAFetchGUI.playersList) {
            if (player.id == id) {
                player.name = name;
                player.country = country;
                player.club = club;
                player.age = age;
                break;
            }
        }
        FIFAFetchGUI.showPlayersButtons();
    }


}
