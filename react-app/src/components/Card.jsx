import React from "react";
import { useState } from "react";
import ButtonElement from "./ButtonElement";
import classes from "./Card.module.css";
import Led from "./Led";

const Card = () => {
  const [led, setLed] = useState(false);
  const handleBtn = () => {
    console.log("data");
    setLed(!led);
  };
  return (
    <div className={classes.card}>
      <h3>Active yes/no</h3>
      <Led status={led} />
      {/* <label htmlFor="hour1">Start Time</label>
      <input type="time" name="hour1" id="hour1" />
      <label htmlFor="hour2">Stop Time</label>
      <input type="time" name="hour2" id="hour2" /> */}
      <input type="range" name="range" id="range" />

      <ButtonElement
        title="Timer"
        status={false}
        on="Start"
        off="Stop"
        onChangeLedStatus={handleBtn}
      />
    </div>
  );
};

export default Card;
