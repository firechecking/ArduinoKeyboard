
void ConvertKeyToHex(String keyStr) {
  Serial.println(keyStr);
  int i, j;
  int keyValue[11] = {};
  for (i = 4; i <= keyStr.length(); i += 4) {
    int subValue = 0;
    int power = 1;
    String subKey = keyStr.substring(i - 4, i);
    //    Serial.println("subKey_"+String(i)+"="+subKey);
    for (j = 0; j < subKey.length(); j++) {
      subValue += subKey.substring(j, j + 1).toInt() * power;
      //      Serial.println("  subValue_"+String(j)+"="+String(subValue));
      power = power * 4;
    }
    if (subValue > 0) {
      keyValue[i / 4 - 1] = subValue;
    }
  }
  //  Serial.print(":");
  for (i = 0; i < 11; i++) {
//        Serial.write(keyValue[i]);
//    Serial.print(keyValue[i]);
//    Serial.print("-");
  }
//  Serial.println();
}
