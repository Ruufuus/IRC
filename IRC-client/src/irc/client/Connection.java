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
import javafx.application.Platform;

/**
 *
 * @author Robert Ciemny
 */
public class Connection implements Runnable {

    private Socket clientSocket = null;
    private BufferedReader reader = null;
    private PrintWriter writer = null;
    private boolean running;
    private final FXMLDocumentController controller;

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

    public Connection(String ip, int port, FXMLDocumentController controller) {
        this.controller = controller;
        try {
            this.clientSocket = new Socket(ip, port);
            this.running = true;
        } catch (IOException ex) {
            Logger.getLogger(Connection.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public void sendMessage(String clientMessage) {
        new Thread() {
            @Override
            public void run() {
                try {
                    writer = new PrintWriter(getClientSocket().getOutputStream(), true);
                    writer.println(clientMessage);
                } catch (IOException ex) {
                    Logger.getLogger(Connection.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }.start();
    }

    @Override
    public void run() {
        
        while (running) {
            try {
                this.reader = new BufferedReader(new InputStreamReader(getClientSocket().getInputStream()));
            } catch (IOException ex) {
                Logger.getLogger(Connection.class.getName()).log(Level.SEVERE, null, ex);
            }
            try {
                String readerLine;
                while ((readerLine = this.reader.readLine()) != null) {
                    System.out.println(readerLine);
                    final String rl = readerLine;
                    Platform.runLater(new Runnable() {
                        @Override
                        public void run() {
                            if (rl.charAt(0) != '$') {
                                controller.appendMessages(rl);
                            } else {
                                controller.commandHandling(rl);
                            }
                        }
                    });

                    Thread.sleep(10);
  
                }
            } catch (IOException ex) {
                System.out.println("Połączenie zakończono");
            } catch (InterruptedException ex) {
                Logger.getLogger(Connection.class.getName()).log(Level.SEVERE, null, ex);
            }
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
