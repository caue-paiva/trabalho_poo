import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.List; // Add this import statement




public class FIFAFetch {
    private String serverAddress;
    private int serverPort;
    
    public FIFAFetch(String serverAddress, int serverPort) {
        this.serverAddress = serverAddress;
        this.serverPort = serverPort;
    }

    public String __auxSendRequest(String request) {
        String response = "";
        try (Socket socket = new Socket(serverAddress, serverPort);
             PrintWriter out_put = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()), true);
             BufferedReader in_put = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            out_put.println(request);
            response = in_put.readLine();
            //System.out.println("RECEBA O JAVA: " + response);

        } catch (Exception e) {
            e.printStackTrace();
        }
        return response;
    }

    public List<FIFAPlayer> sendRequest(String request){
        String return_str = this.__auxSendRequest(request);
        return FIFAPlayer.parsePlayers(return_str);
    }
    

}
