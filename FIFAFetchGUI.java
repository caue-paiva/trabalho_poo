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

    //cria e mostra a GUI
    public static void createAndShowGUI() {
        JFrame frame = new JFrame("FIFA Player Fetch");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(600, 400);

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(8, 2)); // Ajuste para 8 linhas

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

        panel.add(new JLabel());
        panel.add(searchButton);
        panel.add(new JLabel()); // Espaço vazio
        panel.add(showAllButton);

        resultPanel = new JPanel();
        resultPanel.setLayout(new BoxLayout(resultPanel, BoxLayout.Y_AXIS));
        JScrollPane scrollPane = new JScrollPane(resultPanel);

        frame.getContentPane().add(panel, BorderLayout.NORTH);
        frame.getContentPane().add(scrollPane, BorderLayout.CENTER);

        frame.setVisible(true);

        // configura a conexão com o servidor, usa constantes padrão da classe FIFAFetch
        fifaFetch = new FIFAFetch();
    }

    //método para buscar jogadores baseados nos parâmetros do campos de busca, ativado quando o botão de busca é clicado
    private static void searchPlayer() {
        String id = idField.getText(); //pega valores dos campos
        String age = ageField.getText();
        String name = nameField.getText();
        String country = countryField.getText();
        String club = clubField.getText();

        StringBuilder request = new StringBuilder("functionality:3"); //constroi string pra request pro servidor
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

        List<FIFAPlayer> players = fifaFetch.getPlayers(request.toString()); //request pro servido, retorna uma lista de Players
        resultPanel.removeAll(); // limpa resultados anteriores

        if (players == null) {
            resultPanel.add(new JLabel("Jogador(es) não encontrado(s).")); // texto falando que nenhum jogador foi encontrado
        } else {
            FIFAFetchGUI.playersList = players; //seta variável estática com a lista de jogadores
            FIFAFetchGUI.showPlayersButtons(); //atualiza GUI com os jogadores buscados
        }

        resultPanel.revalidate();
        resultPanel.repaint(); //mostra GUI nova
    }

    //método para mostrar todos os jogadores, ativado quando o botão "Show All Players" é clicado
    private static void showAllPlayers() {
        String request = "functionality:2"; // usa a funcionalidade correta para buscar todos os jogadores
        List<FIFAPlayer> players = fifaFetch.getPlayers(request); // request para o servidor para pegar todos os jogadores
        resultPanel.removeAll(); // limpa resultados anteriores

        if (players == null) {
            resultPanel.add(new JLabel("Nenhum jogador encontrado.")); // texto falando que nenhum jogador foi encontrado
        } else {
            FIFAFetchGUI.playersList = players; // seta variável estática com a lista de jogadores
            FIFAFetchGUI.showPlayersButtons(); // atualiza GUI com os jogadores buscados
        }

        resultPanel.revalidate();
        resultPanel.repaint(); // mostra GUI nova
    }

    //função chamada quando alguém clica no botão de cada jogador mostrado na GUI
    private static void openPlayerWindow(FIFAPlayer player) {
        JFrame playerFrame = new JFrame(player.name); //cria frame, seta o tamanho e layout de grid
        playerFrame.setSize(300, 200);
        playerFrame.setLayout(new GridLayout(3, 1));

        JLabel nameLabel = new JLabel("Jogador: " + player.name + " com id: " + player.id); //label de identificação do jogador
        playerFrame.add(nameLabel);

        JButton removeButton = new JButton("Remover Jogador"); //opção de remover jogador
        removeButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                FIFAFetchGUI.removePlayer(player.id, playerFrame);
            }
        });
        playerFrame.add(removeButton); //add botão de remover

        JButton updateButton = new JButton("Update Player");  //opção de atualizar jogador
        updateButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                FIFAFetchGUI.updatePlayer(player, playerFrame); //função de atualizar jogador
                playerFrame.dispose();
            }
        });
        playerFrame.add(updateButton); //add o botão de atualizar 
        playerFrame.setVisible(true); //deixa o frame como visível
    }

    //atualiza a GUI com a lista de botões dos jogadores contidos na lista da variável estática playersList
    private static void showPlayersButtons(){
        resultPanel.removeAll(); //remove os botões antigos
        for (FIFAPlayer player : FIFAFetchGUI.playersList) { //coloca os botões novos
            JButton playerButton = new JButton(player.name);
            playerButton.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    openPlayerWindow(player); //caso alguém clique no botão desse jogador, abre essa janela
                }
            });
            resultPanel.add(playerButton); //add o botão no painel
        }
        resultPanel.revalidate(); //atualiza a GUI
        resultPanel.repaint();
    }

    //método para remover um jogador, faz request pro servidor
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

    //método para atualizar um jogador, faz request pro servidor
    private static void updatePlayer(FIFAPlayer player, JFrame playerFrame){
        JFrame updateFrame = new JFrame("Atualizar jogador");
        updateFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        updateFrame.setSize(400, 300);

        JPanel panel = new JPanel(); //cria novo painel para escrever os campos de atualização
        panel.setLayout(new GridLayout(5, 2)); // layout de grid

        // campo do nome
        JLabel nameLabel = new JLabel("Name:");
        JTextField nameField = new JTextField();
        nameField.setText(player.name); // valor padrão já existente
        panel.add(nameLabel);
        panel.add(nameField);

        // campo da idade
        JLabel ageLabel = new JLabel("Age:");
        JTextField ageField = new JTextField();
        ageField.setText(String.valueOf(player.age)); // valor padrão
        panel.add(ageLabel);
        panel.add(ageField);

        // campo do país
        JLabel countryLabel = new JLabel("Country:");
        JTextField countryField = new JTextField();
        countryField.setText(player.country); // valor padrão
        panel.add(countryLabel);
        panel.add(countryField);

        //campo do clube
        JLabel clubLabel = new JLabel("Club:");
        JTextField clubField = new JTextField();
        clubField.setText(player.club); // valor padrão
        panel.add(clubLabel);
        panel.add(clubField);

        // botão de submeter a atualização
        JButton submitButton = new JButton("Atualizar");
        submitButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) { //função para executar quando o botão de atualizar for clicado
                String name = nameField.getText();
                String country = countryField.getText(); //pega o texto dos campos
                String club = clubField.getText();
                String age = ageField.getText();

                StringBuilder update_request = new StringBuilder("functionality:7"); //cria a string da request pro servidor
                update_request.append(",id:").append(player.id);
                if (!age.isEmpty())
                    update_request.append(",age:").append(age);
                if (!name.isEmpty())
                    update_request.append(",name:").append(name);
                if (!country.isEmpty())
                    update_request.append(",country:").append(country);
                if (!club.isEmpty())
                    update_request.append(",club:").append(club);

                Boolean result = fifaFetch.updatePlayer(update_request.toString()); //manda request para o server
                if (result) { //pop-up de sucesso ou falha
                    JOptionPane.showMessageDialog(playerFrame, "Atualização com sucesso");
                } else {
                    JOptionPane.showMessageDialog(playerFrame, "Atualização falhou");
                }
                FIFAFetchGUI.updatePlayersList(player.id, name, country, club, Integer.parseInt(age)); //atualiza a GUI com a  mudança nova
                updateFrame.dispose();
            }
        });
        panel.add(new JLabel()); 
        panel.add(submitButton); //coloca botão de submeter a atualização
    
        updateFrame.add(panel); 
        updateFrame.setVisible(true); //coloca o frame como visível
    }

    //atualiza a lista de jogadores na GUI com as novas informações
    public static void updatePlayersList(int id, String name, String country, String club, int age) {
        if (playersList == null) { //lista null
            return;
        }
        for (FIFAPlayer player : FIFAFetchGUI.playersList) { //loop pela lista de players
            if (player.id == id) { //achou o cara certo
                player.name = name; //atualiza as informações
                player.country = country;
                player.club = club;
                player.age = age;
                break;
            }
        }
        FIFAFetchGUI.showPlayersButtons(); //atualiza lista de botões na GUI
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
    }
}
