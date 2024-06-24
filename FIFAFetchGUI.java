import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.RoundRectangle2D;
import java.util.List;

class RoundedTextField extends JTextField {
	private Shape shape;

	public RoundedTextField(int size) {
		super(size);
		setOpaque(false); // faz com que o campo seja transparente para vermos as bordas arredondadas
		setBorder(new EmptyBorder(5, 10, 5, 10)); // adicona um padding interno
	}

	protected void paintComponent(Graphics g) {
		Graphics2D g2 = (Graphics2D) g.create();
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		g2.setColor(getBackground());
		g2.fillRoundRect(0, 0, getWidth() - 1, getHeight() - 1, 15, 15); // desenha o campo de texto com bordas arredondadas
		super.paintComponent(g2);
	}

	protected void paintBorder(Graphics g) {
		Graphics2D g2 = (Graphics2D) g.create();
		g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		g2.setColor(getForeground());
		g2.drawRoundRect(0, 0, getWidth() - 1, getHeight() - 1, 15, 15); // desenha a borda arredondada
	}

	public boolean contains(int x, int y) {
		if (shape == null || !shape.getBounds().equals(getBounds())) {
			shape = new RoundRectangle2D.Float(0, 0, getWidth() - 1, getHeight() - 1, 15, 15); // cria uma forma arredondada
		}
		return shape.contains(x, y); // verifica se o ponto está dentro da forma arredondada
	}
}

public class FIFAFetchGUI {

	/*
	Classe para criar a GUI onde o usuário pode interagir com os dados dos jogadores FIFA
	*/

	private static RoundedTextField idField, ageField, nameField, countryField, clubField;
	private static JPanel resultPanel;
	private static FIFAFetch fifaFetch;
	private static List<FIFAPlayer> playersList;
	private static boolean showEditButtons = false; // seta um estado para o botão de edição

	public static void createAndShowGUI() {
		JFrame frame = new JFrame("FIFA Player Fetch");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(1013, 568);

		JPanel panel = new JPanel();
		panel.setLayout(new BorderLayout());

		// texto de boas-vindas com estilo
		JLabel welcomeLabel = new JLabel("<html><body style='text-align: center; font-size: 14px; padding: 10px;'>"
				+ "Welcome to FIFAFetch, here you have access to all players present in the FIFA game, enjoy!"
				+ "</body></html>", JLabel.CENTER);

		// painel para os campos de entrada
		JPanel inputPanel = new JPanel();
		inputPanel.setLayout(new GridLayout(6, 2, 10, 10)); // 6 linhas, 2 colunas, espaçamento de 10px
		inputPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10)); // padding ao redor do painel

		// cria os labels e campos de texto para ID, Age, Name, Country e Club
		JLabel idLabel = new JLabel("ID:");
		idField = new RoundedTextField(15);
		JLabel ageLabel = new JLabel("Age:");
		ageField = new RoundedTextField(15);
		JLabel nameLabel = new JLabel("Name:");
		nameField = new RoundedTextField(15);
		JLabel countryLabel = new JLabel("Country:");
		countryField = new RoundedTextField(15);
		JLabel clubLabel = new JLabel("Club:");
		clubField = new RoundedTextField(15);

		// adiciona os labels e campos ao painel de entrada
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

		// botão de busca com ícone
		JButton searchButton = new JButton(" Search");
		ImageIcon searchIcon = new ImageIcon("imgs/search.png");
		searchButton.setIcon(searchIcon);
		configureButton(searchButton, new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				searchPlayer(); // chama a função de busca ao clicar no botão
			}
		});

		// botão de mostrar todos os jogadores com ícone
		JButton showAllButton = new JButton(" Show All Players");
		ImageIcon playersIcon = new ImageIcon("imgs/players.png");
		showAllButton.setIcon(playersIcon);
		configureButton(showAllButton, new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				showAllPlayers(); // chama a função de mostrar todos os jogadores ao clicar no botão
			}
		});

		// painel para os botões de "Search" e "Show All Players"
		JPanel buttonsPanel = new JPanel(new BorderLayout());

		// adiciona o botão de "Show All Players" ao lado esquerdo do painel
		buttonsPanel.add(showAllButton, BorderLayout.WEST);

		// adiciona o botão de "Search" ao lado direito do painel
		buttonsPanel.add(searchButton, BorderLayout.EAST);

		// adiciona o painel de botões ao painel de entrada
		inputPanel.add(new JLabel()); // adiciona uma célula vazia para espaçamento
		inputPanel.add(buttonsPanel);

		// cria o painel de resultados e adiciona a um JScrollPane
		resultPanel = new JPanel();
		resultPanel.setLayout(new BoxLayout(resultPanel, BoxLayout.Y_AXIS));
		JScrollPane scrollPane = new JScrollPane(resultPanel);
		scrollPane.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10)); // padding ao redor do scrollPane

		// adiciona os componentes ao painel principal
		panel.add(welcomeLabel, BorderLayout.NORTH);
		panel.add(inputPanel, BorderLayout.CENTER);

		// adiciona o painel principal e o painel de resultados ao frame
		frame.getContentPane().add(panel, BorderLayout.NORTH);
		frame.getContentPane().add(scrollPane, BorderLayout.CENTER);

		// torna o frame visível
		frame.setVisible(true);

		// inicializa o objeto fifaFetch
		fifaFetch = new FIFAFetch();
	}

	// função para buscar jogadores com base nos campos de entrada
	private static void searchPlayer() {
		String id = idField.getText();
		String age = ageField.getText();
		String name = nameField.getText();
		String country = countryField.getText();
		String club = clubField.getText();

		// constrói a string de solicitação com os parâmetros preenchidos
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

		// obtém a lista de jogadores correspondentes à solicitação
		List<FIFAPlayer> players = fifaFetch.getPlayers(request.toString());
		resultPanel.removeAll();

		// verifica se algum jogador foi encontrado
		if (players == null) {
			resultPanel.add(new JLabel("Jogador(es) não encontrado(s)."));
		} else {
			FIFAFetchGUI.playersList = players;
			FIFAFetchGUI.showPlayersButtons(); // atualiza a exibição dos jogadores encontrados
		}

		resultPanel.revalidate();
		resultPanel.repaint();
	}

	// função para mostrar todos os jogadores
	private static void showAllPlayers() {
		String request = "functionality:2";
		List<FIFAPlayer> players = fifaFetch.getPlayers(request);
		resultPanel.removeAll();

		// verifica se algum jogador foi encontrado
		if (players == null) {
			resultPanel.add(new JLabel("Nenhum jogador encontrado."));
		} else {
			FIFAFetchGUI.playersList = players;
			FIFAFetchGUI.showPlayersButtons(); // atualiza a exibição de todos os jogadores
		}

		resultPanel.revalidate();
		resultPanel.repaint();
	}

	// função para abrir a janela de detalhes de um jogador
	private static void openPlayerWindow(FIFAPlayer player) {
		JFrame playerFrame = new JFrame(player.name);
		playerFrame.setSize(350, 200);
		playerFrame.setLayout(new BorderLayout());

		// painel principal com boxLayout vertical
		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));

		// imagem do jogador
		ImageIcon playerIcon = new ImageIcon("imgs/player.png");
		JLabel imageLabel = new JLabel(playerIcon);
		imageLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
		mainPanel.add(imageLabel);

		// nome do jogador
		JLabel nameLabel = new JLabel("Jogador: " + player.name);
		nameLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
		mainPanel.add(nameLabel);

		// id do jogador
		JLabel idLabel = new JLabel("ID: " + player.id);
		idLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
		mainPanel.add(idLabel);

		// painel para os botões com boxLayout horizontal
		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));

		// botão de remover
		JButton removeButton = new JButton("  Remove Player");
		ImageIcon trashIcon = new ImageIcon("imgs/trash.png");
		removeButton.setIcon(trashIcon);
		configureButton(removeButton, new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				FIFAFetchGUI.removePlayer(player.id, playerFrame); // chama a função de remover ao clicar no botão
			}
		});
		buttonPanel.add(removeButton);

		// adiciona um espaço rígido entre os botões
		buttonPanel.add(Box.createRigidArea(new Dimension(5, 0)));

		// botão de update
		JButton updateButton = new JButton("  Update Player");
		ImageIcon updateIcon = new ImageIcon("imgs/update.png");
		updateButton.setIcon(updateIcon);
		configureButton(updateButton, new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				FIFAFetchGUI.updatePlayer(player, playerFrame); // chama a função de atualização ao clicar no botão
				playerFrame.dispose(); // fecha a janela do jogador após a atualização
			}
		});
		buttonPanel.add(updateButton);

		buttonPanel.setAlignmentX(Component.CENTER_ALIGNMENT);
		mainPanel.add(buttonPanel);

		playerFrame.add(mainPanel, BorderLayout.CENTER);
		playerFrame.setVisible(true);
	}

	// função para exibir os botões de jogadores encontrados
	private static void showPlayersButtons() {
		resultPanel.removeAll();
		ImageIcon miniPlayerIcon = new ImageIcon("imgs/miniPlayer.png");

		// painel para o título e os botões
		JPanel headerPanel = new JPanel(new BorderLayout());

		// adicionando o subtítulo "Players List"
		JLabel playersListLabel = new JLabel("Players List");
		playersListLabel.setFont(new Font("Arial", Font.BOLD, 16)); // define uma fonte maior e em negrito
		headerPanel.add(playersListLabel, BorderLayout.WEST);

		// painel para os botões
		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new FlowLayout(FlowLayout.RIGHT));

		// botão "Edit Players"
		JButton showEditButton = new JButton(" Edit Players");
		ImageIcon editIcon = new ImageIcon("imgs/edit.png");
		showEditButton.setIcon(editIcon);
		configureButton(showEditButton, new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				showEditButtons = !showEditButtons; // alterna o estado do botão de edição
				showEditButton.setText(showEditButtons ? " Abort Edition" : " Edit Players"); // atualiza o texto do botão de edição
				showPlayersButtons(); // atualiza a exibição dos botões de jogador
			}
		});

		// adiciona os botões ao painel de botões
		buttonPanel.add(showEditButton);

		// adiciona o painel de botões ao painel de cabeçalho
		headerPanel.add(buttonPanel, BorderLayout.EAST);

		// adiciona o painel de cabeçalho ao painel de resultados
		resultPanel.add(headerPanel);

		// adiciona os jogadores
		for (FIFAPlayer player : playersList) {
			JPanel playerPanel = new JPanel(new BorderLayout());

			JLabel playerNameLabel = new JLabel(player.name, miniPlayerIcon, JLabel.LEFT);
			playerNameLabel.setHorizontalAlignment(SwingConstants.LEFT); // nome do jogador sempre a esquerda

			// pegando a imagem do botão de edição
			ImageIcon editIconPlayer = new ImageIcon("imgs/pencil.png");
			JButton editButton = new JButton(editIconPlayer);

			// tornando o botão transparente
			editButton.setOpaque(false);
			editButton.setContentAreaFilled(false);
			editButton.setBorderPainted(false);
			editButton.setFocusPainted(false);

			configureButton(editButton, new ActionListener() {
				@Override
				public void actionPerformed(ActionEvent e) {
					openPlayerWindow(player); // abre a janela do jogador ao clicar no botão
				}
			});

			editButton.setVisible(showEditButtons); // define a visibilidade com base no estado de showEditButtons

			playerPanel.add(playerNameLabel, BorderLayout.WEST);
			playerPanel.add(editButton, BorderLayout.EAST); // botão de editar sempre a direita

			// adiciona espaço extra quando os botões de edição estão ocultos
			if (!showEditButtons) {
				playerPanel.setBorder(BorderFactory.createEmptyBorder(5, 0, 5, 0)); // espaçamento extra quando o botão de edição está oculto
			}

			resultPanel.add(playerPanel);
		}
		resultPanel.revalidate();
		resultPanel.repaint();
	}

	// função para remover um jogador
	private static void removePlayer(int id, JFrame playerFrame) {
		StringBuilder remove_request = new StringBuilder("functionality:5");
		remove_request.append(",id:").append(id);
		Boolean result = fifaFetch.removePlayer(remove_request.toString());
		if (result) {
			JOptionPane.showMessageDialog(playerFrame, "Successful removal!"); // mostra mensagem de sucesso
		} else {
			JOptionPane.showMessageDialog(playerFrame, "Removal failed!"); // mostra mensagem de falha
		}
		playerFrame.dispose();

		// remove o jogador da lista e atualiza a exibição dos botões de jogador
		Boolean resultado = FIFAFetchGUI.playersList.removeIf(player -> player.id == id);
		FIFAFetchGUI.showPlayersButtons();
	}

	// função para atualizar um jogador
	private static void updatePlayer(FIFAPlayer player, JFrame playerFrame) {
		JFrame updateFrame = new JFrame("Update Player");
		updateFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		updateFrame.setSize(400, 300);

		JPanel panel = new JPanel();
		panel.setLayout(new GridLayout(5, 2, 10, 10));
		panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10)); // adiciona padding

		// cria labels e campos de texto para atualização do jogador
		JLabel nameLabel = new JLabel("Name:");
		RoundedTextField nameField = new RoundedTextField(15);
		nameField.setText(player.name);
		panel.add(nameLabel);
		panel.add(nameField);

		JLabel ageLabel = new JLabel("Age:");
		RoundedTextField ageField = new RoundedTextField(15);
		ageField.setText(String.valueOf(player.age));
		panel.add(ageLabel);
		panel.add(ageField);

		JLabel countryLabel = new JLabel("Country:");
		RoundedTextField countryField = new RoundedTextField(15);
		countryField.setText(player.country);
		panel.add(countryLabel);
		panel.add(countryField);

		JLabel clubLabel = new JLabel("Club:");
		RoundedTextField clubField = new RoundedTextField(15);
		clubField.setText(player.club);
		panel.add(clubLabel);
		panel.add(clubField);

		// botão de submissão de atualização
		JButton submitButton = new JButton(" Update");
		ImageIcon updateIcon = new ImageIcon("imgs/update.png");
		submitButton.setIcon(updateIcon);
		submitButton.setHorizontalTextPosition(SwingConstants.RIGHT);
		submitButton.setVerticalTextPosition(SwingConstants.CENTER);
		submitButton.setContentAreaFilled(false);
		submitButton.setBorderPainted(false);
		submitButton.setFocusPainted(false);
		submitButton.setOpaque(false);
		configureButton(submitButton, new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				String name = nameField.getText();
				String country = countryField.getText();
				String club = clubField.getText();
				String age = ageField.getText();

				// constrói a string de solicitação de atualização
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

				// envia a solicitação de atualização e mostra mensagem de sucesso ou falha
				Boolean result = fifaFetch.updatePlayer(update_request.toString());
				if (result) {
					JOptionPane.showMessageDialog(playerFrame, "Update successfully!");
				} else {
					JOptionPane.showMessageDialog(playerFrame, "Update failed!");
				}
				FIFAFetchGUI.updatePlayersList(player.id, name, country, club, Integer.parseInt(age));
				updateFrame.dispose(); // fecha a janela de atualização após a operação
			}
		});

		panel.add(new JLabel()); // adiciona uma célula vazia para espaçamento
		panel.add(submitButton);

		updateFrame.add(panel);
		updateFrame.setVisible(true);
	}

	// função para atualizar a lista de jogadores após uma alteração
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
		FIFAFetchGUI.showPlayersButtons(); // atualiza a exibição dos botões de jogador
	}

	// função para configurar os botões
	private static void configureButton(JButton button, ActionListener actionListener) {
		button.addActionListener(actionListener);
		button.setHorizontalTextPosition(SwingConstants.RIGHT);
		button.setContentAreaFilled(false);
		button.setBorderPainted(false);
		button.setFocusPainted(false);
		button.setOpaque(false);
		button.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseEntered(MouseEvent e) {
				button.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR)); // muda o cursor para mão ao passar sobre o botão
			}

			@Override
			public void mouseExited(MouseEvent e) {
				button.setCursor(Cursor.getDefaultCursor()); // volta o cursor para padrão ao sair do botão
			}
		});
	}

	// função principal para iniciar a aplicação
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				createAndShowGUI(); // chama a função para criar e mostrar a GUI
			}
		});
	}
}
