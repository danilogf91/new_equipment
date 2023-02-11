import React from "react";
import classes from "./TemperaturaHumedad.module.css";

const TemperaturaHumedad = () => {
  return (
    <div className={classes.temperaturaHumedad}>
      Temperature = 27°C - Humidity = 53%
    </div>
  );
};

export default TemperaturaHumedad;
