import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.List; // Add this import statement




public class FIFAFetch {
    private String serverAddress;
    private int serverPort;

    private final int CLIENT_ID = 31787;
    private Boolean serverHasId;
    
    public FIFAFetch(String serverAddress, int serverPort) {
        this.serverAddress = serverAddress;
        this.serverPort = serverPort;
        this.serverHasId = false;
    }

    public String SendRequest(String request) {
        String response = "";
        try (Socket socket = new Socket(serverAddress, serverPort);
             PrintWriter out_put = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()), true);
             BufferedReader in_put = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            if (!this.serverHasId){
                out_put.println(CLIENT_ID); //manda o id do cliente
                this.serverHasId = true;
            }
            out_put.println(request);
            response = in_put.readLine();
            //System.out.println("RECEBA O JAVA: " + response);

        } catch (Exception e) {
            e.printStackTrace();
        }
        return response;
    }

    public List<FIFAPlayer> getPlayers(String request){
        String return_str = this.SendRequest(request);
        return FIFAPlayer.parsePlayers(return_str);
    }

    public Boolean removePlayer(String request){
        String return_str = this.SendRequest(request);
        if (return_str.contains("True")){
            return true;
        } else {
            return false;
        }
    }
    

}
