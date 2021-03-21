boolean reconnect() {
  Serial.print("Reconnecting MQTT: ");
  Serial.println(mqtt_server);
  mqtt_client.connect("CO2ampel");
  return mqtt_client.connected();
}

void publish_values() {
  if (millis() - startMillis < period) {
	  return;
  }
  if (strcmp(mqtt_server, "mqtt.example.org") == 0) {
	  Serial.println("MQTT not configured");
	  return;
  }
  if (!mqtt_client.connected()) {
    Serial.println("Not connected to MQTT yet");
    long now = millis();
    if (now - mqtt_last_reconnect_attempt > 5000) {
      mqtt_last_reconnect_attempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        mqtt_last_reconnect_attempt = 0;
      }
    }
  } else {
    // Client connected
    char buffer[200];
    snprintf(buffer, 200, "{\"co2\": %f, \"temp\": %f, \"hum\": %f, \"light\": %f}", co2.getMedian(), temperatur.getMedian(), luftfeuchte.getMedian(), licht.getMedian());
    Serial.print("Publish: ");
    Serial.println(buffer);
    if (!mqtt_client.publish(mqtt_topic, buffer, true)) {
      // publish failed, prepare for a new connection
      mqtt_client.disconnect();
    }
    startMillis = millis();
  }
}

void mqtt_connect() {
  Serial.print("Connecting to MQTT server ");
  Serial.println(mqtt_server);
  mqtt_client.setServer(mqtt_server, 1883);
  mqtt_last_reconnect_attempt = 0;
}
