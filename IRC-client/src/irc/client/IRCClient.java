/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package irc.client;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

/**
 *
 * @author Robert Ciemny
 */
public class IRCClient extends Application {

    private FXMLDocumentController controller;

    @Override
    public void start(Stage stage) throws Exception {
        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("FXMLDocument.fxml"));
        Parent root = fxmlLoader.load();
        controller = fxmlLoader.getController();

        Scene scene = new Scene(root);
        stage.setTitle("IRC H4XXXOR CLIENT");
        stage.setScene(scene);
        stage.show();
    }

    @Override
    public void stop() throws IOException {
        if (FXMLDocumentController.connection != null) {
            FXMLDocumentController.connection.setRunning(false);

            try {
                FXMLDocumentController.connection.getClientSocket().shutdownInput();
                FXMLDocumentController.connection.getClientSocket().shutdownOutput();
                FXMLDocumentController.connection.getClientSocket().close();
            } catch (IOException ex) {
                Logger.getLogger(Connection.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }

}
