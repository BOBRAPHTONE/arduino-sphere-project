// Plays a full file from beginning to end with no pause.
void playcomplete(char *name) {
  playfile(name);
  while (wave.isplaying) {
  // block
  }
}

void playfile(char *name) {
  if (wave.isplaying) {
    wave.stop();
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    Serial.print("Couldn't open file "); Serial.print(name); return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    Serial.println("Not a valid WAV"); return;
  }
  wave.play();
}
