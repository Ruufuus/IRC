/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package irc.client;

import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.text.TextFlow;

public class FXMLDocumentController {

    @FXML
    private ResourceBundle resources;

    @FXML
    private URL location;

    @FXML
    private TextField messField;

    @FXML
    private TextFlow textFlow;

    @FXML
    private ListView<?> canalList;

    @FXML
    private ListView<?> userList;

    @FXML
    void sendMessage(ActionEvent event) {
        messField.clear();
    }

    @FXML
    void initialize() {
        assert messField != null : "fx:id=\"messField\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert textFlow != null : "fx:id=\"textFlow\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert canalList != null : "fx:id=\"canalList\" was not injected: check your FXML file 'FXMLDocument.fxml'.";
        assert userList != null : "fx:id=\"userList\" was not injected: check your FXML file 'FXMLDocument.fxml'.";

    }
}
