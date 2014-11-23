void sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  Serial.print("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  Serial.print(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

