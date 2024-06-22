import java.util.ArrayList;
import java.util.List;

public class FIFAPlayer {
   public int id;
   public int age;
   public String name;
   public String country;
   public String club;

   public FIFAPlayer(int id, int age, String name, String country, String club) {
       this.id = id;
       this.age = age;
       this.name = name;
       this.country = country;
       this.club = club;
   }

   public FIFAPlayer(){
         this.id = -1;
         this.age = -1;
         this.name = "";
         this.country = "";
         this.club = "";
   }

   @Override
    public String toString() {
        return "{ Nome do Jogador: " + name + " | Nacionalidade do Jogador: " + country + " | Clube do Jogador: " + club + " | ID: " + id +  " | Idade: " + age +" }";
    }

   static List<FIFAPlayer> parsePlayers(String received_message){
        System.out.println(received_message);
        String[] parts = received_message.split("\\|");
        List<FIFAPlayer> lista_joga = new ArrayList<FIFAPlayer>();

        FIFAPlayer cur_player = new FIFAPlayer();

        // Process each part

        for (String part : parts) {
            // Trim the part to remove any leading or trailing whitespace
            part = part.trim();
            

            // Skip empty parts
            if (part.isEmpty()) {
                continue;
            }

            String field_val = part.split(":")[1].trim();

            if (part.contains("Nome")){
                cur_player.name = field_val;   
            } else if (part.contains("Nacionalidade")){
                cur_player.country = field_val;
            } else if (part.contains("Clube")){
                cur_player.club = field_val;
            } else if (part.contains("Id do jogador")){
                cur_player.id = Integer.parseInt(field_val);
            } else if (part.contains("Idade")){
                cur_player.age = Integer.parseInt(field_val);
                lista_joga.add(cur_player);
                cur_player = new FIFAPlayer();
            }
        }
        
            return lista_joga;
        }

   static Boolean updatePlayerInList(List<FIFAPlayer> playerList, int id, FIFAPlayer newInfo){
       for (FIFAPlayer p : playerList){ //loop pela lista de players
           if (p.id == id){ //se o player atual for o player que queremos atualizar
               p.age = newInfo.age; //atualiza campos
               p.name = newInfo.name;
               p.country = newInfo.country;
               p.club = newInfo.club;
               return true;
           }
       }
       return false;

   }

   static Boolean removePlayerFromList(List<FIFAPlayer> playerList,int id){
       for (FIFAPlayer p : playerList){
           if (p.id == id){
               playerList.remove(p);
               return true;
           }
       }
       return false;
   }

}
