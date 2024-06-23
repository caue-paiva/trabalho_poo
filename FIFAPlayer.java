import java.util.ArrayList;
import java.util.List;

public class FIFAPlayer {
    public int id;
    public int age;
    public String name;
    public String country;
    public String club;

    // Construtor da classe com todos os campos
    public FIFAPlayer(int id, int age, String name, String country, String club) {
        this.id = id;
        this.age = age;
        this.name = name;
        this.country = country;
        this.club = club;
    }

    // Construtor da classe sem argumentos, valores padronizados para representar campos vazios
    public FIFAPlayer(){
        this.id = -1;
        this.age = -1;
        this.name = "";
        this.country = "";
        this.club = "";
    }

    // Printa o objeto jogador
    @Override
    public String toString() {
        return "{ Nome do Jogador: " + name + " | Nacionalidade do Jogador: " + country + " | Clube do Jogador: " + club + " | ID: " + id +  " | Idade: " + age + " }";
    }

    // Dado uma string retornada do servidor, dá parsing nela campo a campo e retorna uma lista de objetos FIFAPlayer que representa o retorno
    // da função de busca no arquivo
    static List<FIFAPlayer> parsePlayers(String received_message){
        String[] parts = received_message.split("\\|"); // Split na string pelo delimitador |
        List<FIFAPlayer> lista_joga = new ArrayList<FIFAPlayer>(); // Lista de jogadores

        FIFAPlayer cur_player = new FIFAPlayer(); // Jogador atual

        for (String part : parts) { // Processa cada parte da string retornada pelo server
            part = part.trim(); // Tira whitespace no final e começo

            if (part.isEmpty()) { // Pula strings vazias
                continue;
            }

            String[] keyValue = part.split(":");
            if (keyValue.length < 2) {
                continue; // Ignora partes que não possuem um valor
            }

            String field_name = keyValue[0].trim();
            String field_val = keyValue[1].trim();

            if (field_name.contains("Nome")) { // Verifica qual o campo referido e atualiza o jogador atual
                cur_player.name = field_val;   
            } else if (field_name.contains("Nacionalidade")) {
                cur_player.country = field_val;
            } else if (field_name.contains("Clube")) {
                cur_player.club = field_val;
            } else if (field_name.contains("Id do jogador")) {
                cur_player.id = Integer.parseInt(field_val);
            } else if (field_name.contains("Idade")) { // Último campo, adiciona o jogador atual na lista e cria um novo jogador
                cur_player.age = Integer.parseInt(field_val);
                lista_joga.add(cur_player);
                cur_player = new FIFAPlayer();
            }
        }
        return lista_joga; // Retorna lista de jogadores
    }
}
