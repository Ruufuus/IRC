/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package irc.client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Robert Ciemny
 */
public class Connection implements Runnable{

    private Socket clientSocket = null;
    private BufferedReader reader = null;
    private PrintWriter writer = null;
    private boolean running;
    
    /**
     * @return the clientSocket
     */
    public Socket getClientSocket() {
        return clientSocket;
    }

    /**
     * @param clientSocket the clientSocket to set
     */
    public void setClientSocket(Socket clientSocket) {
        this.clientSocket = clientSocket;
    }
    
    
    public Connection(String ip, int port){
        try {
            this.clientSocket = new Socket(ip, port);
            this.running = true;
        } catch (IOException ex) {
            Logger.getLogger(Connection.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void sendMessage(String clientMessage) throws IOException{
        writer = new PrintWriter(getClientSocket().getOutputStream(), true);
        writer.println(clientMessage);
    }

    @Override
    public void run() {
        while(running){
            try {
                this.reader = new BufferedReader(new InputStreamReader(getClientSocket().getInputStream()));
                System.out.println(reader.readLine());
            } catch (IOException ex) {
                Logger.getLogger(Connection.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        try {
            FXMLDocumentController.connection.getClientSocket().close();
        } catch (IOException ex) {
            Logger.getLogger(Connection.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * @return the isRunning
     */
    public boolean isRunning() {
        return running;
    }

    /**
     * @param isRunning the isRunning to set
     */
    public void setRunning(boolean isRunning) {
        this.running = isRunning;
    }
}
