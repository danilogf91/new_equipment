import React from "react";
import classes from "./Led.module.css";

const Led = ({ status }) => {
  return (
    <div
      className={`${classes.led} ${status ? classes.ledOn : classes.ledOff}`}
    ></div>
  );
};

export default Led;
