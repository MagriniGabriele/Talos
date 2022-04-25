uint8_t getFingerprintID(){
  uint8_t P = finger.getImage();
  switch(P){
    case FINGERPRINT_OK:
      Serial.println("Impronta letta");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Nessun dito rilevato");
      return P;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Errore di comunicazione");
      return P;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Impronta non valida");
      return P;
    default:
      Serial.println("Errore sconosciuto");
      return P;
  }

  // OK successo!

  P = finger.image2Tz();
  switch(P){
    case FINGERPRINT_OK:
      Serial.println("Impronta convertita");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Impronta non valida (disordinata)");
      return P;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Errore di comunicazione");
      return P;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Impossibile trovare le carattestistiche dell'impronta");
      return P;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Impossibile trovare le carattestistiche dell'impronta");
      return P;
    default:
      Serial.println("Errore sconosciuto");
      return P;
  }
  
  // OK converted!
  P = finger.fingerFastSearch();
  if(P == FINGERPRINT_OK) {
    Serial.println("Trovata una corrispondenza");
  } else if (P == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Errore di comunicazione");
    return P;
  } else if (P == FINGERPRINT_NOTFOUND) {
    Serial.println("Nessuna corrispondenza trovata");
    return P;
  } else {
    Serial.println("Errore sconosciuto");
    return P;
  } 
  //Trovata corrispondenza
  Serial.print("Impronta rilevata ID #");
  Serial.println(finger.fingerID); 
  return finger.fingerID;
  
}

//Restituisce -1 se fallito altrimenti l'ID
int getFingerprintIDez() {
  uint8_t P = finger.getImage();
  if (P != FINGERPRINT_OK)  return -1;

  P = finger.image2Tz();
  if (P != FINGERPRINT_OK)  return -1;

  P = finger.fingerFastSearch();
  if (P != FINGERPRINT_OK)  return -1;
  
  //Trovata corrispondenza
  if(!blackList.Contains(finger.fingerID)){
    Serial.print("Impronta rilevata ID #");
    Serial.println(finger.fingerID);
    Status = !Status;
    return finger.fingerID;
  }
  
  return -1;
  
}
