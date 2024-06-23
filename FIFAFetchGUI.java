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
    private static boolean showEditButtons = false; // Rastrea o estado do botão de edição

    public static void createAndShowGUI() {
        JFrame frame = new JFrame("FIFA Player Fetch");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(600, 400);

        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());

        // Texto de boas-vindas com estilo
        JLabel welcomeLabel = new JLabel("<html><body style='text-align: center; font-size: 14px; padding: 10px;'>"
                + "Welcome to FIFAFetch, here you have access to all players present in the FIFA game, enjoy!"
                + "</body></html>", JLabel.CENTER);

        // Painel para os campos de entrada
        JPanel inputPanel = new JPanel();
        inputPanel.setLayout(new GridLayout(8, 2, 10, 10));
        inputPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10)); // Padding ao redor do painel

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

        inputPanel.add(idLabel);
        inputPanel.add(idField);
        inputPanel.add(ageLabel);
        inputPanel.add(ageField);
        inputPanel.add(nameLabel);
        inputPanel.add(nameField);
        inputPanel.add(countryLabel);
        inputPanel.add(countryField);
        inputPanel.add(clubLabel);
        inputPanel.add(clubField);

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

        JButton showEditButton = new JButton("Edit Players");
        showEditButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                showEditButtons = !showEditButtons; // Alterna o estado do botão de edição
                showEditButton.setText(showEditButtons ? "Abort Edition" : "Edit Players"); // Atualiza o texto do botão de edição
                showPlayersButtons(); // Atualiza a exibição dos botões de jogador
            }
        });

        inputPanel.add(new JLabel());
        inputPanel.add(searchButton);
        inputPanel.add(new JLabel());
        inputPanel.add(showAllButton);
        inputPanel.add(new JLabel());
        inputPanel.add(showEditButton);

        resultPanel = new JPanel();
        resultPanel.setLayout(new BoxLayout(resultPanel, BoxLayout.Y_AXIS));
        JScrollPane scrollPane = new JScrollPane(resultPanel);
        scrollPane.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10)); // Padding ao redor do scrollPane

        panel.add(welcomeLabel, BorderLayout.NORTH);
        panel.add(inputPanel, BorderLayout.CENTER);

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
        playerFrame.setSize(350, 200);
        playerFrame.setLayout(new BorderLayout());

        // Cria um painel principal com BoxLayout vertical
        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));

        // Cria o rótulo para a imagem do jogador
        ImageIcon playerIcon = new ImageIcon("imgs/player.png");
        JLabel imageLabel = new JLabel(playerIcon);
        imageLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        mainPanel.add(imageLabel);

        // Cria o rótulo para o nome do jogador
        JLabel nameLabel = new JLabel("Jogador: " + player.name);
        nameLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        mainPanel.add(nameLabel);

        // Cria o rótulo para o id do jogador
        JLabel idLabel = new JLabel("ID: " + player.id);
        idLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        mainPanel.add(idLabel);

        // Cria um painel para os botões com BoxLayout horizontal
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));

        // Cria o botão de remover com ícone
        JButton removeButton = new JButton("  Remove Player");
        ImageIcon trashIcon = new ImageIcon("imgs/trash.png");
        removeButton.setIcon(trashIcon);
        removeButton.setHorizontalTextPosition(SwingConstants.RIGHT);
        removeButton.setContentAreaFilled(false);
        removeButton.setBorderPainted(false);
        removeButton.setFocusPainted(false);
        removeButton.setOpaque(false);
        removeButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                FIFAFetchGUI.removePlayer(player.id, playerFrame);
            }
        });
        buttonPanel.add(removeButton);

        // Adiciona um espaço rígido entre os botões
        buttonPanel.add(Box.createRigidArea(new Dimension(5, 0)));

        // Cria o botão de update com ícone
        JButton updateButton = new JButton("  Update Player");
        ImageIcon updateIcon = new ImageIcon("imgs/update.png");
        updateButton.setIcon(updateIcon);
        updateButton.setHorizontalTextPosition(SwingConstants.RIGHT);
        updateButton.setContentAreaFilled(false);
        updateButton.setBorderPainted(false);
        updateButton.setFocusPainted(false);
        updateButton.setOpaque(false);
        updateButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                FIFAFetchGUI.updatePlayer(player, playerFrame);
                playerFrame.dispose();
            }
        });
        buttonPanel.add(updateButton);

        buttonPanel.setAlignmentX(Component.CENTER_ALIGNMENT);
        mainPanel.add(buttonPanel);

        playerFrame.add(mainPanel, BorderLayout.CENTER);
        playerFrame.setVisible(true);
    }

    private static void showPlayersButtons() {
        resultPanel.removeAll();
        ImageIcon miniPlayerIcon = new ImageIcon("imgs/miniPlayer.png");
        
        for (FIFAPlayer player : playersList) {
            JPanel playerPanel = new JPanel(new BorderLayout());
            
            JLabel playerNameLabel = new JLabel(player.name, miniPlayerIcon, JLabel.LEFT);
            playerNameLabel.setHorizontalAlignment(SwingConstants.LEFT); // Nome do jogador sempre a esquerda

            // Pegando a imagem do botão de edição
            ImageIcon editIcon = new ImageIcon("imgs/pencil.png");
            JButton editButton = new JButton(editIcon);

            // Tornar o botão transparente
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

            editButton.setVisible(showEditButtons); // Define a visibilidade com base no estado de showEditButtons

            playerPanel.add(playerNameLabel, BorderLayout.WEST);
            playerPanel.add(editButton, BorderLayout.EAST); // Botão de editar sempre a direita

            // Adiciona espaço extra quando os botões de edição estão ocultos
            if (!showEditButtons) {
                playerPanel.setBorder(BorderFactory.createEmptyBorder(5, 0, 5, 0)); // Espaçamento extra quando o botão de edição está oculto
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
            JOptionPane.showMessageDialog(playerFrame, "Successful removal!");
        } else {
            JOptionPane.showMessageDialog(playerFrame, "Removal failed!");
        }
        playerFrame.dispose();

        Boolean resultado = FIFAFetchGUI.playersList.removeIf(player -> player.id == id);
        System.out.println(resultado);
        FIFAFetchGUI.showPlayersButtons();
    }

    private static void updatePlayer(FIFAPlayer player, JFrame playerFrame) {
        JFrame updateFrame = new JFrame("Update Player");
        updateFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        updateFrame.setSize(400, 300);

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(5, 2, 10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10)); // Adiciona padding

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

        JButton submitButton = new JButton("Update");
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
                    JOptionPane.showMessageDialog(playerFrame, "Update successfully!");
                } else {
                    JOptionPane.showMessageDialog(playerFrame, "Update failed!");
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
