/**
 * Sample Skeleton for 'UserWindow.fxml' Controller Class
 */

package irc.client;

import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;

public class UserWindowController {
    
    private FXMLDocumentController parentController;
    
    private String username;
    
    
    void setParentController(FXMLDocumentController controller){
        this.parentController = controller;
    }

    @FXML // ResourceBundle that was given to the FXMLLoader
    private ResourceBundle resources;

    @FXML // URL location of the FXML file that was given to the FXMLLoader
    private URL location;

    @FXML // fx:id="okUserButton"
    private Button okUserButton; // Value injected by FXMLLoader

    @FXML // fx:id="anulujUserButton"
    private Button anulujUserButton; // Value injected by FXMLLoader

    @FXML
    void anulujUserButtonAction(ActionEvent event) {

    }

    @FXML
    void okUserButtonAction(ActionEvent event) {

    }

    @FXML // This method is called by the FXMLLoader when initialization is complete
    void initialize() {
        assert okUserButton != null : "fx:id=\"okUserButton\" was not injected: check your FXML file 'UserWindow.fxml'.";
        assert anulujUserButton != null : "fx:id=\"anulujUserButton\" was not injected: check your FXML file 'UserWindow.fxml'.";

    }
}
