import React from "react";
import classes from "./ButtonElement.module.css";
import { useState } from "react";

const ButtonElement = ({ title, status, onChangeLedStatus, on, off }) => {
  const [ledStatus, setLedStatus] = useState(status);

  const handleGpio = () => {
    setLedStatus(!ledStatus);
    onChangeLedStatus();
  };

  return (
    <button className={classes.btn} onClick={handleGpio}>
      {!ledStatus ? on : off} = {title}
    </button>
  );
};

export default ButtonElement;
