/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package irc.client;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.text.TextFlow;
import javafx.scene.control.Button;
import javafx.scene.control.MenuItem;
import javafx.stage.Stage;

public class FXMLDocumentController {
    
    ///////////////////////////////////////

    Connection connection = null;
    
    ///////////////////////////////////////
    @FXML
    private ResourceBundle resources;

    @FXML
    private URL location;

    @FXML
    private TextField messField;

    @FXML
    private TextFlow textFlow;

    @FXML
    private MenuItem polaczSubMenu;

    @FXML
    private ListView<?> canalList;

    @FXML
    private ListView<?> userList;

    @FXML
    void sendMessage(ActionEvent event) {
        messField.clear();
    }

    @FXML
    private TextField adresTextField;

    @FXML
    private TextField portTextField;

    @FXML
    private Button okConnectionButton;

    @FXML
    private Button anulujConnectionButton;

    @FXML
    void anulujConnectionButtonAction(ActionEvent event) {

    }

    @FXML
    void okConnectionButtonAction(ActionEvent event) {
        this.connection = new Connection(this.adresTextField.getText(), Integer.parseInt(this.portTextField.getText()));
        Stage stage = (Stage) this.okConnectionButton.getScene().getWindow();
        stage.close();
    }

    @FXML
    void polaczAction(ActionEvent event) {
        try {
            FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("ConnectionWindow.fxml"));
            Parent root = fxmlLoader.load();
            Stage stage = new Stage();
            stage.setTitle("Połącz");
            stage.setScene(new Scene(root));
            stage.show();
        } catch (IOException ex) {
            Logger.getLogger(FXMLDocumentController.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @FXML
    void kanalAction(ActionEvent event) {
        try {
            FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("CanalWindow.fxml"));
            Parent root = fxmlLoader.load();
            Stage stage = new Stage();
            stage.setTitle("Zarządzanie Kanałami");
            stage.setScene(new Scene(root));
            stage.show();
        } catch (IOException ex) {
            Logger.getLogger(FXMLDocumentController.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    @FXML
    private Button okCanalButton;

    @FXML
    private Button anulujCanalButton;

    @FXML
    private TextField textFieldCanal;

    @FXML
    void anulujCanalButtonAction(ActionEvent event) {

    }

    @FXML
    void okCanalButtonAction(ActionEvent event) {

    }

    @FXML
    void textFieldCanalAction(ActionEvent event) {

    }


    @FXML
    void initialize() {

    }
}
