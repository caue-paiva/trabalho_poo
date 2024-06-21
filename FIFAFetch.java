import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

public class FIFAFetch {
    private String serverAddress;
    private int serverPort;
    
    public FIFAFetch(String serverAddress, int serverPort) {
        this.serverAddress = serverAddress;
        this.serverPort = serverPort;
    }

    public String sendRequest(String request) {
        String response = "";
        try (Socket socket = new Socket(serverAddress, serverPort);
             PrintWriter out_put = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()), true);
             BufferedReader in_put = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            out_put.println(request);
            response = in_put.readLine();
            System.out.println(response);

        } catch (Exception e) {
            e.printStackTrace();
        }
        return response;
    }
}
