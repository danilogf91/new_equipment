import React from "react";
import classes from "./CardOutput.module.css";
import Led from "./Led";
import ButtonElement from "./ButtonElement";
import { useState } from "react";

const CardOutput = ({ name, ledTitle, status }) => {
  const [led, setLed] = useState(status);
  const handleLed = () => {
    console.log("led active");
    setLed(!led);
  };
  return (
    <div className={classes.card}>
      <h3>{name}</h3>
      <h3>
        {led && <span className={classes.on}>ON </span>}
        {!led && <span className={classes.off}>OFF</span>}
      </h3>
      <Led status={led} />
      <ButtonElement
        title={ledTitle}
        status={led}
        on="ON"
        off="OFF"
        onChangeLedStatus={handleLed}
      />
    </div>
  );
};

export default CardOutput;
