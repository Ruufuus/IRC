/**
 * Sample Skeleton for 'CanalWindow.fxml' Controller Class
 */

package irc.client;

import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

public class CanalWindowController {
    
    private FXMLDocumentController parentController;
    
    void setParentController(FXMLDocumentController controller){
        this.parentController = controller;
    }

    @FXML // ResourceBundle that was given to the FXMLLoader
    private ResourceBundle resources;

    @FXML // URL location of the FXML file that was given to the FXMLLoader
    private URL location;

    @FXML // fx:id="okCanalButton"
    private Button okCanalButton; // Value injected by FXMLLoader

    @FXML // fx:id="anulujCanalButton"
    private Button anulujCanalButton; // Value injected by FXMLLoader

    @FXML // fx:id="textFieldCanal"
    private TextField textFieldCanal; // Value injected by FXMLLoader

    @FXML
    void anulujCanalButtonAction(ActionEvent event) {
        Stage stage = (Stage) this.anulujCanalButton.getScene().getWindow();
        this.parentController.setKanalWindow(false);
        stage.close();
    }

    @FXML
    void okCanalButtonAction(ActionEvent event) {
        
         if (parentController.getConnection() != null) {
            String message = this.textFieldCanal.getText();
            parentController.getConnection().sendMessage("$join " + message);
            this.textFieldCanal.clear();
            this.parentController.getMessageList().getItems().clear();
        }
        
        Stage stage = (Stage) this.okCanalButton.getScene().getWindow();
        this.parentController.setKanalWindow(false);
        stage.close();
    }

    @FXML
    void textFieldCanalAction(ActionEvent event) {
        this.okCanalButtonAction(event);
    }

    @FXML // This method is called by the FXMLLoader when initialization is complete
    void initialize() {
        assert okCanalButton != null : "fx:id=\"okCanalButton\" was not injected: check your FXML file 'CanalWindow.fxml'.";
        assert anulujCanalButton != null : "fx:id=\"anulujCanalButton\" was not injected: check your FXML file 'CanalWindow.fxml'.";
        assert textFieldCanal != null : "fx:id=\"textFieldCanal\" was not injected: check your FXML file 'CanalWindow.fxml'.";

    }
}
