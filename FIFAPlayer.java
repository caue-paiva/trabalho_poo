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

   static List<FIFAPlayer> parsePlayers(String received_message){
        return null;
    }

}
