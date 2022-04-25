uint8_t getFingerprintEnroll(){
  int P = -1;
  lcd.setCursor(0, 0);
  lcd.print("Registering:");
  Serial.print("In attesa di un dito valido per memorizzarlo come ID#"); Serial.println(id);
  while (P != FINGERPRINT_OK) {
    lcd.setCursor(0, 1);
    lcd.print("");
    lcd.print("Mettere un dito");
    P = finger.getImage();
    switch (P) {
    case FINGERPRINT_OK:
      lcd.setCursor(0, 1);
      lcd.print("");
      lcd.print("Impronta letta");
      Serial.println("Impronta letta");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      lcd.setCursor(0, 1);
      lcd.print("waiting...");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Errore di comunicazione");
      lcd.setCursor(0, 1);
      lcd.print("Errore! Riprova");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Impronta non valida");
      lcd.setCursor(0, 1);
      lcd.print("Errore! Riprova");
      break;
    default:
      lcd.setCursor(0, 1);
      lcd.print("Errore! Riprova");
      Serial.println("Errore sconosciuto");
      break;
    }
  }

  // OK success!

  P = finger.image2Tz(1);
  switch (P) {
    case FINGERPRINT_OK:
      Serial.println("Impronta convertita");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Impronta non valida (disordinata)");
      bluetooth.println("Error");
      return P;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Errore di comunicazione");
      bluetooth.println("Error");
      return P;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Impossibile trovare le carattestistiche dell'impronta");
      bluetooth.println("Error");
      return P;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Impossibile trovare le carattestistiche dell'impronta");
      bluetooth.println("Error");
      return P;
    default:
      Serial.println("Errore sconosciuto");
      bluetooth.println("Error");
      return P;
  }
  lcd.setCursor(0, 1);
  lcd.print("");
  lcd.print("Rimuovi il dito");
  Serial.println("Rimuovi il dito");
  
  delay(2000);
  P = 0;
  while (P != FINGERPRINT_NOFINGER) {
    P = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  P = -1;
  Serial.println("Posiziona di nuovo lo stesso dito");
  lcd.setCursor(0, 1);
  lcd.print("");
  lcd.print("Rimetti il dito");
  while (P != FINGERPRINT_OK) {
    P = finger.getImage();
    switch (P) {
    case FINGERPRINT_OK:
      Serial.println("Impronta letta");
      lcd.setCursor(0, 1);
      lcd.print("");
      lcd.print("Impronta letta");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      lcd.setCursor(0, 1);
      lcd.print("waiting...");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Errore di comunicazione");
      lcd.setCursor(0, 1);
      lcd.print("Errore! Riprova");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Impronta non valida");
      lcd.setCursor(0, 1);
      lcd.print("Errore! Riprova");
      break;
    default:
      lcd.setCursor(0, 1);
      lcd.print("Errore! Riprova");
      Serial.println("Errore sconosciuto");
      break;
    }
  }

  // OK success!

  P = finger.image2Tz(2);
  switch (P) {
    case FINGERPRINT_OK:
      Serial.println("Impronta convertita");
      lcd.setCursor(0, 1);
      lcd.print("");
      lcd.print("Dito convertito");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Impronta non valida (disordinata)");
      bluetooth.println("Error");
      return P;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Errore di comunicazione");
      bluetooth.println("Error");
      return P;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Impossibile trovare le carattestistiche dell'impronta");
      bluetooth.println("Error");
      return P;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Impossibile trovare le carattestistiche dell'impronta");
      bluetooth.println("Error");
      return P;
    default:
      Serial.println("Errore sconosciuto");
      bluetooth.println("Error");
      return P;
  }
  
  // OK converted!
  Serial.print("Creazione dell'impronta per l'ID#");
  Serial.println(id);
  
  P = finger.createModel();
  if (P == FINGERPRINT_OK) {
    Serial.println("Impronte abbinate!");
  } else if (P == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Errore di comunicazione");
    bluetooth.println("Error");
    return P;
  } else if (P == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Le impronte digitali non corrispondono");
    bluetooth.println("Error");
    return P;
  } else {
    Serial.println("Errore sconosciuto");
    bluetooth.println("Error");
    return P;
  }   
  
  Serial.print("ID "); Serial.println(id);
  P = finger.storeModel(id);
  if (P == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.setCursor(0, 1);
    lcd.print("");
    lcd.println("Successo!");
    bluetooth.print("S");
  } else if (P == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Errore di comunicazione");
    bluetooth.println("Error");
    return P;
  } else if (P == FINGERPRINT_BADLOCATION) {
    Serial.println("Impossibile archiviare quella posizione");
    bluetooth.println("Error");
    return P;
  } else if (P == FINGERPRINT_FLASHERR) {
    Serial.println("Errore durante la scrittura");
    bluetooth.println("Error");
    return P;
  } else {
    Serial.println("Errore sconosciuto");
    bluetooth.println("Error");
    return P;
  }   
  lcd.setCursor(0, 0);
  lcd.print("");
  lcd.setCursor(0, 1);
  lcd.print("");
  return P;
}
