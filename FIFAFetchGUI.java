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
    private static boolean showEditButtons = false; // rastrea o estado do botão de edição

    public static void createAndShowGUI() {
        JFrame frame = new JFrame("FIFA Player Fetch");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(600, 400);

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(8, 2, 10, 10)); 
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10)); // padding ao redor do painel

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

        JButton showAllButton = new JButton("Show All Players");
        showAllButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                showAllPlayers();
            }
        });

        JButton showEditButton = new JButton("Show Edit");
        showEditButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                showEditButtons = !showEditButtons; //alterna o estado do botão de edição
                showPlayersButtons(); // atualiza a exibição dos botões
        });

        panel.add(new JLabel());
        panel.add(searchButton);
        panel.add(new JLabel());
        panel.add(showAllButton);
        panel.add(new JLabel());
        panel.add(showEditButton);

        resultPanel = new JPanel();
        resultPanel.setLayout(new BoxLayout(resultPanel, BoxLayout.Y_AXIS));
        JScrollPane scrollPane = new JScrollPane(resultPanel);
        scrollPane.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10)); // padding ao redor do scrollPane

        frame.getContentPane().add(panel, BorderLayout.NORTH);
        frame.getContentPane().add(scrollPane, BorderLayout.CENTER);

        frame.setVisible(true);

        fifaFetch = new FIFAFetch();
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
        resultPanel.removeAll();

        if (players == null) {
            resultPanel.add(new JLabel("Jogador(es) não encontrado(s)."));
        } else {
            FIFAFetchGUI.playersList = players;
            FIFAFetchGUI.showPlayersButtons();
        }

        resultPanel.revalidate();
        resultPanel.repaint();
    }

    private static void showAllPlayers() {
        String request = "functionality:2";
        List<FIFAPlayer> players = fifaFetch.getPlayers(request);
        resultPanel.removeAll();

        if (players == null) {
            resultPanel.add(new JLabel("Nenhum jogador encontrado."));
        } else {
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

        JLabel nameLabel = new JLabel("Jogador: " + player.name + " com id: " + player.id);
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

    private static void showPlayersButtons() {
        resultPanel.removeAll();
        for (FIFAPlayer player : playersList) {
            JPanel playerPanel = new JPanel(new BorderLayout());
            JLabel playerNameLabel = new JLabel(player.name);
            playerNameLabel.setHorizontalAlignment(SwingConstants.LEFT); // nome do jogador sempre a esquerda

            // Carregar a imagem do ícone
            ImageIcon editIcon = new ImageIcon("imgs/pencil.png");
            JButton editButton = new JButton(editIcon);

            // Configurações para tornar o botão transparente
            editButton.setOpaque(false);
            editButton.setContentAreaFilled(false);
            editButton.setBorderPainted(false);
            editButton.setFocusPainted(false);

            editButton.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    openPlayerWindow(player);
                }
            });

            editButton.setVisible(showEditButtons); // define a visibilidade dos botões pelo showEditButtons

            playerPanel.add(playerNameLabel, BorderLayout.WEST);
            playerPanel.add(editButton, BorderLayout.EAST); // botão de editar sempre a direita

            // adicionando espaço extra quando os botões de edição estão ocultos
            if (!showEditButtons) {
                playerPanel.setBorder(BorderFactory.createEmptyBorder(9, 0, 9, 0)); // expaãmento extra pra ficar mais estético
            }

            resultPanel.add(playerPanel);
        }
        resultPanel.revalidate();
        resultPanel.repaint();
    }

    private static void removePlayer(int id, JFrame playerFrame) {
        StringBuilder remove_request = new StringBuilder("functionality:5");
        remove_request.append(",id:").append(id);
        Boolean result = fifaFetch.removePlayer(remove_request.toString());
        if (result) {
            JOptionPane.showMessageDialog(playerFrame, "Remoção com sucesso");
        } else {
            JOptionPane.showMessageDialog(playerFrame, "Remoção falhou");
        }
        playerFrame.dispose();

        Boolean resultado = FIFAFetchGUI.playersList.removeIf(player -> player.id == id);
        System.out.println(resultado);
        FIFAFetchGUI.showPlayersButtons();
    }

    private static void updatePlayer(FIFAPlayer player, JFrame playerFrame) {
        JFrame updateFrame = new JFrame("Atualizar jogador");
        updateFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        updateFrame.setSize(400, 300);

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(5, 2, 10, 10)); // 5 linhas, 2 colunas, 10 de espaçamento horizontal e vertical

        JLabel nameLabel = new JLabel("Name:");
        JTextField nameField = new JTextField();
        nameField.setText(player.name);
        panel.add(nameLabel);
        panel.add(nameField);

        JLabel ageLabel = new JLabel("Age:");
        JTextField ageField = new JTextField();
        ageField.setText(String.valueOf(player.age));
        panel.add(ageLabel);
        panel.add(ageField);

        JLabel countryLabel = new JLabel("Country:");
        JTextField countryField = new JTextField();
        countryField.setText(player.country);
        panel.add(countryLabel);
        panel.add(countryField);

        JLabel clubLabel = new JLabel("Club:");
        JTextField clubField = new JTextField();
        clubField.setText(player.club);
        panel.add(clubLabel);
        panel.add(clubField);

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
                FIFAFetchGUI.updatePlayersList(player.id, name, country, club, Integer.parseInt(age));
                updateFrame.dispose();
            }
        });
        panel.add(new JLabel());
        panel.add(submitButton);

        updateFrame.add(panel);
        updateFrame.setVisible(true);
    }

    public static void updatePlayersList(int id, String name, String country, String club, int age) {
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

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
    }
}
