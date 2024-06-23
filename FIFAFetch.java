import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.List;

public class FIFAFetch {
    private String serverAddress;
    private int serverPort;
    private  int client_id;

    private static final int PORT = 9090; // constantes padrão para o port do socket e o ip do localhost
    private static final String SERVER_ADDRESS = "127.0.0.1";
    private Boolean serverHasId;

    // construtor que usa os valores padrão da classe para o port e o endereço de ip
    // (localhost)
    public FIFAFetch() {
        this.serverAddress = FIFAFetch.SERVER_ADDRESS;
        this.serverPort = FIFAFetch.PORT;
        this.serverHasId = false;
        this.client_id = (int) (Math.random() * 1000); // gera um id aleatório para o cliente
    }

    // construtor que recebe o endereço de ip e o port do servidor
    public FIFAFetch(String serverAddress, int serverPort) {
        this.serverAddress = serverAddress;
        this.serverPort = serverPort;
        this.serverHasId = false;
        this.client_id = (int) (Math.random() * 1000); // gera um id aleatório para o cliente
    }

    private String sendRequest(String request) {
        String response = "";
        try (Socket socket = new Socket(this.serverAddress, this.serverPort);
                PrintWriter out_put = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()), true);
                BufferedReader in_put = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            if (!this.serverHasId) { // server não tem o id do cliente
                out_put.println(this.client_id); // manda o id do cliente
                out_put.flush();
                Thread.sleep(1500); // da sleep para essa mensagem ser processada como uma mensagem separada pelo
                                    // python
                this.serverHasId = true;
            }
            out_put.println(request);
            out_put.flush();
            response = in_put.readLine();

        } catch (Exception e) {
            e.printStackTrace();
        }
        return response;
    }

    public List<FIFAPlayer> getPlayers(String request) {
        String return_str = this.sendRequest(request);
        System.out.println("Mensagem recebida do servidor: " + return_str);
        if (return_str == null || return_str.isEmpty() || return_str.contains("inexistente")) {
            return null;
        }

        // Substituir " Nacionalidade" por " | Nacionalidade"
        if (return_str != null) {
            return_str = return_str.replaceFirst("  Nacionalidade", " | Nacionalidade");
        }

        return FIFAPlayer.parsePlayers(return_str);
    }

    public Boolean removePlayer(String request) {
        if (!request.contains("5")) { // 5 é o código da funcionalidade de remoção
            return false;
        }

        String return_str = this.sendRequest(request);
        if (return_str == null) {
            return false;
        }
        if (return_str.contains("True")) {
            return true;
        } else {
            return false;
        }
    }

    public Boolean updatePlayer(String request) {
        if (!request.contains("7")) { // 7 é o código da funcionalidade da atualização
            return false;
        }
        System.out.println(request);

        String return_str = this.sendRequest(request);
        System.out.println(return_str);
        if (return_str == null) {
            return false;
        }
        if (return_str.contains("True")) {
            return true;
        } else {
            return false;
        }
    }
}
