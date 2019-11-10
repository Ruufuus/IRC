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
    Socket clientSocket = null;
    BufferedReader reader = null;
    PrintWriter writer = null;
    
    
    
    public Connection(String ip, int port){
        try {
            this.clientSocket = new Socket(ip, port);
        } catch (IOException ex) {
            Logger.getLogger(Connection.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void sendMessage(String clientMessage) throws IOException{
        writer = new PrintWriter(clientSocket.getOutputStream(), true);
        writer.println(clientMessage);
    }

    @Override
    public void run() {
        while(true){
            BufferedReader reader;
            try {
                reader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                System.out.println(reader.readLine());
            } catch (IOException ex) {
                Logger.getLogger(Connection.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}
