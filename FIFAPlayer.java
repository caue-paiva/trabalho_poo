import java.util.ArrayList;
import java.util.List;

public class FIFAPlayer {
   public int id;
   public int age;
   public String name;
   public String country;
   public String club;

   //construtor da classe com todos os campos
   public FIFAPlayer(int id, int age, String name, String country, String club) {
       this.id = id;
       this.age = age;
       this.name = name;
       this.country = country;
       this.club = club;
   }

   //construtor da classe sem argumentos, valores padronizados para representar campos vazios
   public FIFAPlayer(){
         this.id = -1;
         this.age = -1;
         this.name = "";
         this.country = "";
         this.club = "";
   }

   //printa o objeto jogador
   @Override
    public String toString() {
        return "{ Nome do Jogador: " + name + " | Nacionalidade do Jogador: " + country + " | Clube do Jogador: " + club + " | ID: " + id +  " | Idade: " + age +" }";
    }

   //dado uma string retornada do servidor, dá parsing nela campo a campo e retorna uma lista de objetos FIFAPlayer que representa o retorno
   //da função de busca no arquivo
   static List<FIFAPlayer> parsePlayers(String received_message){

        String[] parts = received_message.split("\\|"); //split na string pelo delimitador |
        List<FIFAPlayer> lista_joga = new ArrayList<FIFAPlayer>(); //lista de jogadores

        FIFAPlayer cur_player = new FIFAPlayer(); //jogador atual

        for (String part : parts) { //processa cada parte da string  retornada pelo server
            part = part.trim(); //tira whitespace no final e começo
            
            if (part.isEmpty()) { //pula strings vazias
                continue;
            }

            String field_val = part.split(":")[1].trim(); //pega o valor do campo

            if (part.contains("Nome")){ //ve qual o campo referido a atualiza o jogador atual
                cur_player.name = field_val;   
            } else if (part.contains("Nacionalidade")){
                cur_player.country = field_val;
            } else if (part.contains("Clube")){
                cur_player.club = field_val;
            } else if (part.contains("Id do jogador")){
                cur_player.id = Integer.parseInt(field_val);
            } else if (part.contains("Idade")){ //ultimo campo, vamos adicionar o jogador atual na lista e criar um novo jogador
                cur_player.age = Integer.parseInt(field_val);
                lista_joga.add(cur_player);
                cur_player = new FIFAPlayer();
            }
        }
            return lista_joga; //retorna lista de jogadores
        }

}
