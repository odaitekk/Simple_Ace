#include "Calibration.h"
#include "SPIFFS.h"
#include "Simple_ACE.h"
#include <EEPROM.h>
#include <Adafruit_ADS1X15.h>
#include "TFT_eSPI.h"
#include "Image_assets/Loading.h"
#include "PID.h"


extern TFT_eSPI tft;
extern Adafruit_ADS1115 ads;
const int waittime =1000;

extern short Sensor_arr[store_size];
// // extern short Sensor_arr[store_size];
// short Sensor_arr_numb[] = {8509 ,8513 ,8511 ,8515 ,8515 ,8511 ,8511 ,8511 ,8514 ,8511 ,8510 ,8510 ,8514 ,8514 ,8511 ,8510 ,8510 ,8514 ,8514 ,8513 ,8510 ,8515 ,8515 ,8512 ,8513 ,8510 ,8513 ,8512 ,8514 ,8510 ,8515 ,8514 ,8510 ,8512 ,8511 ,8514 ,8513 ,8514 ,8515 ,8511 ,8510 ,8515 ,8511 ,8511 ,8514 ,8513 ,8514 ,8512 ,8509 ,8513 ,8510 ,8509 ,8509 ,8512 ,8512 ,8514 ,8512 ,8511 ,8511 ,8510 ,8512 ,8512 ,8509 ,8508 ,8510 ,8509 ,8512 ,8513 ,8512 ,8508 ,8509 ,8513 ,8512 ,8513 ,8510 ,8509 ,8510 ,8513 ,8509 ,8513 ,8512 ,8508 ,8510 ,8513 ,8512 ,8511 ,8512 ,8509 ,8510 ,8510 ,8513 ,8508 ,8512 ,8512 ,8510 ,8507 ,8511 ,8509 ,8510 ,8510 ,8511 ,8512 ,8507 ,8510 ,8508 ,8506 ,8508 ,8511 ,8507 ,8508 ,8509 ,8506 ,8505 ,8508 ,8510 ,8513 ,8510 ,8511 ,8508 ,8512 ,8507 ,8510 ,8507 ,8507 ,8511 ,8507 ,8513 ,8509 ,8507 ,8506 ,8510 ,8506 ,8508 ,8509 ,8508 ,8510 ,8508 ,8510 ,8510 ,8506 ,8506 ,8507 ,8508 ,8508 ,8507 ,8505 ,8506 ,8505 ,8509 ,8506 ,8510 ,8506 ,8509 ,8511 ,8508 ,8507 ,8506 ,8508 ,8505 ,8512 ,8507 ,8508 ,8505 ,8505 ,8508 ,8508 ,8511 ,8506 ,8507 ,8509 ,8508 ,8506 ,8507 ,8509 ,8508 ,8510 ,8509 ,8512 ,8510 ,8510 ,8507 ,8508 ,8513 ,8512 ,8513 ,8511 ,8511 ,8514 ,8511 ,8513 ,8515 ,8515 ,8514 ,8519 ,8515 ,8515 ,8516 ,8517 ,8521 ,8517 ,8519 ,8522 ,8519 ,8520 ,8527 ,8521 ,8526 ,8527 ,8526 ,8526 ,8529 ,8533 ,8529 ,8526 ,8533 ,8534 ,8529 ,8532 ,8534 ,8533 ,8537 ,8535 ,8537 ,8537 ,8537 ,8536 ,8540 ,8540 ,8538 ,8538 ,8539 ,8539 ,8542 ,8544 ,8541 ,8545 ,8548 ,8546 ,8546 ,8549 ,8550 ,8548 ,8548 ,8547 ,8548 ,8553 ,8552 ,8554 ,8555 ,8551 ,8552 ,8553 ,8553 ,8553 ,8554 ,8554 ,8554 ,8557 ,8555 ,8558 ,8557 ,8555 ,8555 ,8555 ,8559 ,8554 ,8555 ,8554 ,8558 ,8555 ,8554 ,8553 ,8554 ,8555 ,8556 ,8552 ,8557 ,8553 ,8557 ,8554 ,8556 ,8553 ,8548 ,8550 ,8551 ,8548 ,8552 ,8548 ,8552 ,8551 ,8547 ,8550 ,8547 ,8547 ,8549 ,8543 ,8548 ,8546 ,8542 ,8544 ,8545 ,8546 ,8546 ,8541 ,8545 ,8545 ,8543 ,8545 ,8542 ,8544 ,8540 ,8544 ,8543 ,8541 ,8539 ,8541 ,8537 ,8536 ,8538 ,8537 ,8537 ,8535 ,8537 ,8539 ,8535 ,8539 ,8535 ,8534 ,8535 ,8536 ,8536 ,8531 ,8536 ,8531 ,8531 ,8536 ,8534 ,8535 ,8535 ,8533 ,8532 ,8530 ,8528 ,8532 ,8530 ,8532 ,8531 ,8529 ,8531 ,8529 ,8526 ,8531 ,8531 ,8526 ,8530 ,8530 ,8526 ,8526 ,8524 ,8525 ,8525 ,8527 ,8525 ,8528 ,8524 ,8526 ,8525 ,8522 ,8526 ,8526 ,8523 ,8526 ,8525 ,8523 ,8522 ,8525 ,8521 ,8522 ,8524 ,8521 ,8523 ,8520 ,8523 ,8520 ,8519 ,8523 ,8522 ,8522 ,8522 ,8522 ,8522 ,8518 ,8523 ,8518 ,8521 ,8522 ,8520 ,8519 ,8518 ,8522 ,8517 ,8517 ,8519 ,8517 ,8518 ,8519 ,8520 ,8519 ,8521 ,8516 ,8517 ,8518 ,8516 ,8520 ,8521 ,8520 ,8516 ,8516 ,8517 ,8520 ,8515 ,8519 ,8515 ,8518 ,8517 ,8518 ,8514 ,8515 ,8516 ,8514 ,8517 ,8516 ,8514 ,8516 ,8511 ,8517 ,8517 ,8518 ,8517 ,8517 ,8514 ,8518 ,8515 ,8516 ,8518 ,8514 ,8513 ,8517 ,8514 ,8518 ,8516 ,8517 ,8513 ,8514 ,8512 ,8514 ,8516 ,8512 ,8516 ,8515 ,8514 ,8514 ,8514 ,8513 ,8513 ,8513 ,8512 ,8516 ,8513 ,8513 ,8510 ,8513 ,8510 ,8512 ,8514 ,8515 ,8515 ,8512 ,8513 ,8512 ,8514 ,8509 ,8512 ,8513 ,8510 ,8514 ,8509 ,8507 ,8511 ,8509 ,8511 ,8509 ,8513 ,8510 ,8511 ,8512 ,8512 ,8507 ,8513 ,8512 ,8509 ,8510 ,8508 ,8511 ,8510 ,8510 ,8508 ,8507 ,8510 ,8510 ,8509 ,8508 ,8513 ,8513 ,8508 ,8508 ,8507 ,8512 ,8513 ,8510 ,8509 ,8512 ,8507 ,8509 ,8510 ,8510 ,8509 ,8508 ,8508 ,8509 ,8511 ,8511 ,8508 ,8506 ,8506 ,8512 ,8508 ,8512 ,8509 ,8511 ,8510 ,8507 ,8508 ,8507 ,8508 ,8511 ,8508 ,8511 ,8513 ,8510 ,8508 ,8506 ,8506 ,8510 ,8508 ,8510 ,8508 ,8510 ,8510 ,8509 ,8509 ,8507 ,8507 ,8508 ,8508 ,8511 ,8508 ,8510 ,8506 ,8509 ,8506 ,8510 ,8507 ,8511 ,8509 ,8506 ,8507 ,8508 ,8508 ,8508 ,8508 ,8508 ,8507 ,8509 ,8506 ,8509 ,8505 ,8507 ,8509 ,8508 ,8506 ,8506 ,8506 ,8509 ,8508 ,8510 ,8507 ,8510 ,8508 ,8508 ,8509 ,8504 ,8506 ,8507 ,8509 ,8507 ,8508 ,8507 ,8505 ,8507 ,8507 ,8508 ,8509 ,8506 ,8507 ,8505 ,8506 ,8510 ,8507 ,8508 ,8507 ,8505 ,8506 ,8508 ,8510 ,8506 ,8505 ,8505 ,8507 ,8506 ,8508 ,8510 ,8505 ,8508 ,8509 ,8508 ,8505 ,8506 ,8509 ,8505 ,8510 ,8505 ,8511 ,8508 ,8508 ,8504 ,8510 ,8505 ,8511 ,8504 ,8508 ,8505 ,8508 ,8506 ,8508 ,8506 ,8509 ,8506 ,8504 ,8509 ,8505 ,8506 ,8507 ,8507 ,8506 ,8506 ,8507 ,8506 ,8504 ,8507 ,8508 ,8509 ,8508 ,8509 ,8510 ,8505 ,8508 ,8505 ,8507 ,8506 ,8507 ,8511 ,8509 ,8506 ,8509 ,8509 ,8506 ,8508 ,8510 ,8508 ,8507 ,8511 ,8507 ,8508 ,8506 ,8508 ,8507 ,8507 ,8506 ,8509 ,8507 ,8507 ,8510 ,8510 ,8506 ,8506 ,8505 ,8506 ,8511 ,8508 ,8511 ,8510 ,8512 ,8510 ,8509 ,8509 ,8510 ,8510 ,8507 ,8509 ,8508 ,8512 ,8510 ,8508 ,8512 ,8511 ,8508 ,8511 ,8508 ,8506 ,8511 ,8512 ,8511 ,8510 ,8511 ,8511 ,8508 ,8511 ,8511 ,8512 ,8508 ,8507 ,8509 ,8511 ,8508 ,8510 ,8508 ,8509 ,8512 ,8513 ,8512 ,8511 ,8508 ,8510 ,8509 ,8511 ,8513 ,8511 ,8513 ,8508 ,8514 ,8511 ,8508 ,8512 ,8513 ,8513 ,8511 ,8510 ,8512 ,8511 ,8509 ,8514 ,8509 ,8514 ,8508 ,8513 ,8513 ,8510 ,8509 ,8513 ,8510 ,8511 ,8514 ,8512 ,8513 ,8513 ,8514 ,8511 ,8513 ,8515 ,8514 ,8513 ,8513 ,8516 ,8514 ,8513 ,8515 ,8516 ,8515 ,8515 ,8514 ,8511 ,8513 ,8516 ,8513 ,8514 ,8513 ,8512 ,8515 ,8511 ,8516 ,8518 ,8513 ,8511 ,8511 ,8515 ,8512 ,8515 ,8516 ,8516 ,8517 ,8514 ,8513 ,8514 ,8516 ,8515 ,8516 ,8513 ,8514 ,8517 ,8514 ,8514 ,8515 ,8514 ,8515 ,8515 ,8519 ,8514 ,8514 ,8517 ,8519 ,8516 ,8519 ,8519 ,8517 ,8515 ,8516 ,8520 ,8515 ,8519 ,8515 ,8518 ,8514 ,8520 ,8516 ,8517 ,8520 ,8518 ,8521 ,8519 ,8516 ,8520 ,8517 ,8521 ,8517 ,8519 ,8517 ,8516 ,8521 ,8518 ,8521 ,8516 ,8520 ,8517 ,8520 ,8520 ,8522 ,8518 ,8519 ,8520 ,8518 ,8522 ,8517 ,8523 ,8522 ,8518 ,8523 ,8524 ,8517 ,8523 ,8523 ,8521 ,8520 ,8523 ,8522 ,8521 ,8521 ,8523 ,8524 ,8520 ,8518 ,8523 ,8523 ,8524 ,8519 ,8520 ,8523 ,8520 ,8525 ,8521 ,8525 ,8523 ,8525 ,8522 ,8522 ,8526 ,8524 ,8524 ,8525 ,8526 ,8526 ,8524 ,8525 ,8526 ,8523 ,8522 ,8523 ,8523 ,8527 ,8524 ,8524 ,8524 ,8528 ,8527 ,8525 ,8524 ,8524 ,8528 ,8527 ,8524 ,8525 ,8528 ,8525 ,8528 ,8525 ,8526 ,8524 ,8525 ,8528 ,8526 ,8525 ,8525 ,8530 ,8530 ,8525 ,8528 ,8527 ,8527 ,8529 ,8530 ,8528 ,8530 ,8528 ,8532 ,8531 ,8533 ,8527 ,8533 ,8528 ,8532 ,8529 ,8530 ,8531 ,8529 ,8531 ,8533 ,8533 ,8530 ,8531 ,8528 ,8530 ,8533 ,8533 ,8532 ,8529 ,8529 ,8532 ,8532 ,8534 ,8530 ,8535 ,8535 ,8534 ,8534 ,8534 ,8530 ,8535 ,8531 ,8532 ,8534 ,8535 ,8531 ,8532 ,8530 ,8535 ,8536 ,8531 ,8536 ,8536 ,8536 ,8533 ,8535 ,8533 ,8533 ,8536 ,8535 ,8535 ,8534 ,8535 ,8534 ,8538 ,8535 ,8534 ,8538 ,8535 ,8534 ,8534 ,8537 ,8536 ,8538 ,8534 ,8538 ,8536 ,8537 ,8536 ,8535 ,8537 ,8535 ,8539 ,8535 ,8536 ,8535 ,8537 ,8537 ,8539 ,8538 ,8540 ,8538 ,8538 ,8537 ,8538 ,8539 ,8540 ,8537 ,8543 ,8539 ,8535 ,8538 ,8541 ,8541 ,8538 ,8541 ,8540 ,8543 ,8544 ,8543 ,8541 ,8541 ,8543 ,8543 ,8539 ,8543 ,8540 ,8541 ,8540 ,8542 ,8542 ,8544 ,8542 ,8541 ,8540 ,8544 ,8543 ,8541 ,8540 ,8546 ,8544 ,8543 ,8543 ,8542 ,8544 ,8543 ,8542 ,8546 ,8540 ,8541 ,8545 ,8546 ,8540 ,8546 ,8544 ,8544 ,8544 ,8547 ,8547 ,8541 ,8545 ,8543 ,8545 ,8545 ,8544 ,8547 ,8549 ,8549 ,8545 ,8547 ,8548 ,8546 ,8548 ,8548 ,8544 ,8547 ,8549 ,8544 ,8548 ,8546 ,8545 ,8550 ,8550 ,8547 ,8547 ,8548 ,8550 ,8546 ,8551 ,8552 ,8551 ,8547 ,8551 ,8551 ,8552 ,8548 ,8549 ,8552 ,8550 ,8549 ,8550 ,8549 ,8549 ,8550 ,8553 ,8552 ,8550 ,8554 ,8549 ,8550 ,8551 ,8554 ,8550 ,8552 ,8551 ,8551 ,8554 ,8553 ,8550 ,8552 ,8551 ,8552 ,8554 ,8554 ,8550 ,8554 ,8555 ,8550 ,8552 ,8552 ,8555 ,8552 ,8552 ,8554 ,8556 ,8550 ,8554 ,8556 ,8555 ,8553 ,8553 ,8552 ,8556 ,8554 ,8553 ,8552 ,8556 ,8558 ,8554 ,8555 ,8558 ,8558 ,8558 ,8553 ,8554 ,8558 ,8558 ,8555 ,8557 ,8554 ,8555 ,8556 ,8556 ,8556 ,8558 ,8555 ,8556 ,8554 ,8555 ,8557 ,8556 ,8556 ,8556 ,8558 ,8558 ,8558 ,8555 ,8556 ,8560 ,8557 ,8560 ,8558 ,8556 ,8557 ,8556 ,8559 ,8557 ,8562 ,8558 ,8561 ,8560 ,8557 ,8558 ,8558 ,8562 ,8557 ,8560 ,8562 ,8563 ,8563 ,8559 ,8561 ,8562 ,8561 ,8558 ,8558 ,8561 ,8559 ,8559 ,8560 ,8561 ,8559 ,8562 ,8559 ,8561 ,8560 ,8562 ,8563 ,8561 ,8558 ,8561 ,8562 ,8559 ,8561 ,8563 ,8560 ,8564 ,8561 ,8559 ,8560 ,8562 ,8564 ,8562 ,8564 ,8565 ,8564 ,8561 ,8565 ,8564 ,8562 ,8565 ,8563 ,8562 ,8564 ,8561 ,8562 ,8562 ,8565 ,8565 ,8563 ,8563 ,8566 ,8562 ,8566 ,8564 ,8566 ,8563 ,8565 ,8564 ,8567 ,8564 ,8567 ,8567 ,8566 ,8568 ,8563 ,8565 ,8563 ,8567 ,8567 ,8566 ,8568 ,8568 ,8567 ,8566 ,8564 ,8568 ,8568 ,8567 ,8568 ,8566 ,8564 ,8567 ,8565 ,8567 ,8565 ,8569 ,8570 ,8566 ,8570 ,8568 ,8570 ,8567 ,8569 ,8569 ,8567 ,8570 ,8566 ,8566 ,8567 ,8567 ,8568 ,8566 ,8569 ,8567 ,8570 ,8570 ,8567 ,8567 ,8570 ,8570 ,8568 ,8566 ,8571 ,8570 ,8571 ,8570 ,8566 ,8570 ,8568 ,8567 ,8568 ,8572 ,8570 ,8568 ,8571 ,8569 ,8567 ,8569 ,8569 ,8569 ,8573 ,8570 ,8570 ,8569 ,8571 ,8568 ,8569 ,8567 ,8570 ,8570 ,8569 ,8574 ,8571 ,8572 ,8569 ,8571 ,8567 ,8572 ,8568 ,8571 ,8571 ,8570 ,8568 ,8569 ,8570 ,8571 ,8567 ,8569 ,8572 ,8571 ,8570 ,8571 ,8573 ,8569 ,8570 ,8572 ,8569 ,8572 ,8571 ,8571 ,8574 ,8572 ,8570 ,8569 ,8574 ,8574 ,8571 ,8567 ,8568 ,8571 ,8571 ,8569 ,8569 ,8571 ,8573 ,8572 ,8571 ,8570 ,8569 ,8569 ,8571 ,8571 ,8574 ,8574 ,8570 ,8570 ,8571 ,8573 ,8573 ,8574 ,8573 ,8571 ,8570 ,8570 ,8570 ,8574 ,8569 ,8574 ,8574 ,8574 ,8570 ,8570 ,8568 ,8572 ,8571 ,8569 ,8571 ,8572 ,8571 ,8572 ,8570 ,8571 ,8573 ,8570 ,8571 ,8572 ,8573 ,8571 ,8571 ,8575 ,8570 ,8572 ,8572 ,8573 ,8571 ,8573 ,8570 ,8572 ,8572 ,8570 ,8573 ,8571 ,8570 ,8574 ,8573 ,8572 ,8570 ,8571 ,8571 ,8570 ,8571 ,8576 ,8574 ,8570 ,8573 ,8574 ,8574 ,8570 ,8573 ,8571 ,8575 ,8575 ,8575 ,8571 ,8571 ,8572 ,8570 ,8571 ,8575 ,8572 ,8576 ,8573 ,8575 ,8572 ,8572 ,8571 ,8571 ,8576 ,8576 ,8572 ,8576 ,8574 ,8572 ,8575 ,8570 ,8575 ,8573 ,8573 ,8571 ,8571 ,8571 ,8573 ,8574 ,8573 ,8575 ,8575 ,8576 ,8574 ,8575 ,8575 ,8572 ,8571 ,8574 ,8570 ,8571 ,8574 ,8573 ,8571 ,8575 ,8573 ,8575 ,8571 ,8574 ,8574 ,8574 ,8574 ,8573 ,8573 ,8572 ,8574 ,8574 ,8572 ,8571 ,8574 ,8572 ,8576 ,8571 ,8573 ,8573 ,8575 ,8572 ,8575 ,8571 ,8572 ,8575 ,8576 ,8575 ,8571 ,8571 ,8572 ,8572 ,8572 ,8576 ,8571 ,8572 ,8574 ,8573 ,8572 ,8575 ,8572 ,8573 ,8576 ,8574 ,8576 ,8575 ,8574 ,8575 ,8574 ,8572 ,8573 ,8572 ,8574 ,8573 ,8572 ,8574 ,8570 ,8574 ,8574 ,8575 ,8574 ,8573 ,8571 ,8576 ,8572 ,8571 ,8572 ,8571 ,8572 ,8575 ,8576 ,8576 ,8571 ,8574 ,8574 ,8574 ,8572 ,8571 ,8575 ,8572 ,8574 ,8575 ,8572 ,8572 ,8571 ,8573 ,8575 ,8570 ,8574 ,8570 ,8574 ,8574 ,8574 ,8570 ,8575 ,8570 ,8571 ,8573 ,8573 ,8572 ,8575 ,8571 ,8575 ,8572 ,8574 ,8571 ,8574 ,8574 ,8573 ,8573 ,8574 ,8573 ,8572 ,8571 ,8576 ,8575 ,8570 ,8570 ,8574 ,8576 ,8571 ,8573 ,8571 ,8572 ,8571 ,8572 ,8573 ,8572 ,8571 ,8576 ,8572 ,8570 ,8575 ,8571 ,8569 ,8572 ,8569 ,8572 ,8573 ,8574 ,8574 ,8568 ,8571 ,8571 ,8573 ,8572 ,8573 ,8571 ,8573 ,8574 ,8571 ,8574 ,8573 ,8571 ,8573 ,8571 ,8573 ,8572 ,8574 ,8570 ,8573 ,8570 ,8574 ,8573 ,8571 ,8575 ,8573 ,8572 ,8569 ,8575 ,8577 ,8571 ,8572 ,8575 ,8574 ,8571 ,8574 ,8570 ,8573 ,8573 ,8573 ,8573 ,8574 ,8571 ,8569 ,8569 ,8574 ,8574 ,8570 ,8572 ,8570 ,8573 ,8573 ,8573 ,8573 ,8571 ,8569 ,8574 ,8567 ,8570 ,8571 ,8571 ,8570 ,8571 ,8573 ,8569 ,8571 ,8572 ,8566 ,8571 ,8567 ,8570 ,8573 ,8568 ,8568 ,8570 ,8566 ,8571 ,8567 ,8569 ,8571 ,8567 ,8571 ,8573 ,8570 ,8572 ,8570 ,8568 ,8570 ,8567 ,8569 ,8570 ,8573 ,8568 ,8570 ,8571 ,8571 ,8567 ,8571 ,8571 ,8570 ,8567 ,8571 ,8570 ,8570 ,8568 ,8569 ,8571 ,8571 ,8573 ,8567 ,8568 ,8570 ,8570 ,8568 ,8573 ,8569 ,8568 ,8569 ,8569 ,8571 ,8568 ,8567 ,8569 ,8569 ,8571 ,8569 ,8566 ,8567 ,8566 ,8571 ,8571 ,8567 ,8572 ,8570 ,8573 ,8571 ,8572 ,8570 ,8568 ,8566 ,8570 ,8571 ,8571 ,8571 ,8568 ,8567 ,8568 ,8569 ,8568 ,8569 ,8567 ,8568 ,8569 ,8567 ,8572 ,8570 ,8570 ,8568 ,8566 ,8570 ,8569 ,8568 ,8566 ,8565 ,8569 ,8566 ,8565 ,8568 ,8564 ,8565 ,8567 ,8568 ,8565 ,8568 ,8567 ,8564 ,8568 ,8565 ,8564 ,8566 ,8567 ,8567 ,8565 ,8567 ,8569 ,8566 ,8568 ,8566 ,8565 ,8567 ,8566 ,8564 ,8565 ,8562 ,8567 ,8567 ,8569 ,8564 ,8566 ,8566 ,8568 ,8565 ,8563 ,8564 ,8567 ,8563 ,8567 ,8564 ,8567 ,8566 ,8565 ,8566 ,8564 ,8567 ,8564 ,8567 ,8562 ,8566 ,8561 ,8567 ,8564 ,8565 ,8563 ,8565 ,8565 ,8561 ,8567 ,8566 ,8561 ,8562 ,8564 ,8566 ,8563 ,8564 ,8566 ,8566 ,8561 ,8565 ,8565 ,8562 ,8565 ,8561 ,8564 ,8565 ,8564 ,8562 ,8562 ,8562 ,8564 ,8563 ,8561 ,8564 ,8563 ,8560 ,8564 ,8561 ,8560 ,8561 ,8561 ,8561 ,8564 ,8561 ,8564 ,8562 ,8561 ,8565 ,8559 ,8561 ,8564 ,8560 ,8564 ,8558 ,8562 ,8563 ,8562 ,8559 ,8560 ,8564 ,8561 ,8558 ,8563 ,8561 ,8559 ,8559 ,8556 ,8560 ,8558 ,8562 ,8563 ,8562 ,8561 ,8561 ,8562 ,8562 ,8559 ,8558 ,8559 ,8559 ,8557 ,8561 ,8559 ,8557 ,8560 ,8556 ,8557 ,8556 ,8559 ,8559 ,8560 ,8559 ,8558 ,8559 ,8561 ,8557 ,8557 ,8560 ,8555 ,8560 ,8561 ,8556 ,8561 ,8559 ,8558 ,8559 ,8554 ,8557 ,8556 ,8556 ,8558 ,8556 ,8553 ,8555 ,8560 ,8555 ,8560 ,8560 ,8558 ,8560 ,8558 ,8556 ,8560 ,8557 ,8558 ,8558 ,8555 ,8555 ,8560 ,8559 ,8558 ,8554 ,8555 ,8559 ,8553 ,8553 ,8556 ,8557 ,8555 ,8556 ,8557 ,8554 ,8556 ,8553 ,8553 ,8558 ,8558 ,8552 ,8556 ,8557 ,8556 ,8556 ,8558 ,8555 ,8555 ,8555 ,8557 ,8555 ,8556 ,8557 ,8554 ,8555 ,8552 ,8553 ,8554 ,8552 ,8558 ,8555 ,8556 ,8551 ,8553 ,8554 ,8556 ,8553 ,8554 ,8555 ,8556 ,8553 ,8552 ,8557 ,8552 ,8552 ,8558 ,8552 ,8553 ,8556 ,8553 ,8551 ,8552 ,8552 ,8552 ,8556 ,8553 ,8552 ,8553 ,8555 ,8553 ,8553 ,8553 ,8551 ,8555 ,8549 ,8552 ,8554 ,8551 ,8551 ,8552 ,8550 ,8551 ,8552 ,8548 ,8550 ,8554 ,8552 ,8551 ,8552 ,8549 ,8550 ,8554 ,8549 ,8553 ,8552 ,8549 ,8551 ,8553 ,8552 ,8551 ,8553 ,8552 ,8552 ,8548 ,8552 ,8553 ,8547 ,8553 ,8552 ,8549 ,8552 ,8553 ,8550 ,8552 ,8554 ,8552 ,8553 ,8550 ,8553 ,8553 ,8554 ,8551 ,8551 ,8550 ,8548 ,8549 ,8549 ,8551 ,8551 ,8550 ,8551 ,8554 ,8551 ,8551 ,8551 ,8550 ,8548 ,8548 ,8548 ,8548 ,8551 ,8552 ,8550 ,8552 ,8548 ,8547 ,8552 ,8549 ,8548 ,8551 ,8552 ,8548 ,8552 ,8547 ,8549 ,8548 ,8551 ,8547 ,8548 ,8551 ,8550 ,8553 ,8551 ,8548 ,8546 ,8549 ,8549 ,8546 ,8549 ,8546 ,8547 ,8551 ,8549 ,8547 ,8550 ,8549 ,8548 ,8546 ,8547 ,8548 ,8546 ,8548 ,8548 ,8547 ,8546 ,8549 ,8547 ,8550 ,8547 ,8546 ,8546 ,8549 ,8548 ,8549 ,8549 ,8550 ,8549 ,8546 ,8547 ,8546 ,8548 ,8549 ,8548 ,8546 ,8546 ,8545 ,8545 ,8547 ,8546 ,8546 ,8546 ,8550 ,8545 ,8545 ,8546 ,8544 ,8549 ,8545 ,8547 ,8546 ,8549 ,8544 ,8546 ,8548 ,8546 ,8549 ,8545 ,8548 ,8548 ,8546 ,8545 ,8546 ,8543 ,8544 ,8545 ,8545 ,8544 ,8542 ,8545 ,8544 ,8544 ,8545 ,8541 ,8543 ,8544 ,8541 ,8544 ,8544 ,8544 ,8543 ,8546 ,8543 ,8544 ,8542 ,8544 ,8543 ,8545 ,8542 ,8540 ,8546 ,8543 ,8540 ,8545 ,8542 ,8542 ,8544 ,8544 ,8542 ,8542 ,8542 ,8540 ,8545 ,8543 ,8540 ,8543 ,8540 ,8544 ,8545 ,8540 ,8543 ,8541 ,8546 ,8541 ,8541 ,8541 ,8543 ,8541 ,8541 ,8540 ,8542 ,8540 ,8541 ,8542 ,8546 ,8543 ,8545 ,8546 ,8541 ,8541 ,8541 ,8542 ,8542 ,8544 ,8541 ,8543 ,8542 ,8540 ,8541 ,8541 ,8541 ,8541 ,8539 ,8544 ,8541 ,8539 ,8540 ,8542 ,8543 ,8541 ,8542 ,8540 ,8539 ,8539 ,8542 ,8540 ,8539 ,8541 ,8542 ,8541 ,8543 ,8542 ,8538 ,8541 ,8541 ,8542 ,8539 ,8540 ,8540 ,8538 ,8538 ,8538 ,8537 ,8535 ,8536 ,8541 ,8539 ,8540 ,8540 ,8537 ,8536 ,8539 ,8541 ,8540 ,8541 ,8538 ,8538 ,8541 ,8540 ,8537 ,8537 ,8536 ,8538 ,8538 ,8539 ,8539 ,8538 ,8539 ,8540 ,8541 ,8539 ,8535 ,8536 ,8535 ,8538 ,8539 ,8538 ,8537 ,8541 ,8536 ,8538 ,8536 ,8538 ,8541 ,8537 ,8536 ,8536 ,8538 ,8535 ,8540 ,8539 ,8536 ,8538 ,8537 ,8536 ,8538 ,8538 ,8538 ,8538 ,8537 ,8535 ,8536 ,8534 ,8538 ,8539 ,8539 ,8538 ,8539 ,8535 ,8534 ,8536 ,8537 ,8539 ,8538 ,8535 ,8537 ,8538 ,8540 ,8537 ,8538 ,8534 ,8536 ,8537 ,8535 ,8536 ,8534 ,8536 ,8536 ,8534 ,8536 ,8537 ,8535 ,8536 ,8538 ,8533 ,8536 ,8535 ,8533 ,8537 ,8533 ,8533 ,8534 ,8535 ,8537 ,8535 ,8532 ,8537 ,8537 ,8536 ,8536 ,8531 ,8534 ,8535 ,8537 ,8537 ,8536 ,8532 ,8536 ,8537 ,8537 ,8534 ,8534 ,8532 ,8532 ,8532 ,8533 ,8533 ,8536 ,8535 ,8531 ,8532 ,8532 ,8533 ,8535 ,8534 ,8529 ,8535 ,8536 ,8534 ,8533 ,8533 ,8531 ,8535 ,8534 ,8533 ,8532 ,8530 ,8533 ,8534 ,8534 ,8533 ,8531 ,8534 ,8533 ,8533 ,8533 ,8534 ,8534 ,8532 ,8530 ,8533 ,8530 ,8531 ,8532 ,8534 ,8533 ,8531 ,8533 ,8532 ,8531 ,8534 ,8530 ,8532 ,8534 ,8529 ,8533 ,8533 ,8533 ,8532 ,8533 ,8533 ,8532 ,8533 ,8529 ,8530 ,8534 ,8530 ,8530 ,8534 ,8534 ,8530 ,8533 ,8528 ,8530 ,8532 ,8530 ,8530 ,8534 ,8532 ,8532 ,8529 ,8530 ,8527 ,8533 ,8533 ,8528 ,8532 ,8532 ,8531 ,8530 ,8528 ,8529 ,8529 ,8529 ,8529 ,8528 ,8527 ,8527 ,8532 ,8530 ,8527 ,8532 ,8531 ,8529 ,8533 ,8530 ,8529 ,8528 ,8531 ,8529 ,8529 ,8532 ,8529 ,8528 ,8527 ,8527 ,8530 ,8529 ,8527 ,8531 ,8531 ,8527 ,8529 ,8530 ,8530 ,8526 ,8529 ,8528 ,8531 ,8532 ,8527 ,8529 ,8531 ,8529 ,8528 ,8527 ,8531 ,8526 ,8527 ,8531 ,8527 ,8527 ,8530 ,8529 ,8531 ,8531 ,8528 ,8527 ,8531 ,8528 ,8526 ,8530 ,8528 ,8529 ,8528 ,8527 ,8525 ,8527 ,8528 ,8526 ,8531 ,8525 ,8528 ,8530 ,8529 ,8525 ,8530 ,8530 ,8525 ,8526 ,8527 ,8524 ,8529 ,8527 ,8527 ,8530 ,8528 ,8526 ,8527 ,8527 ,8526 ,8524 ,8526 ,8529 ,8525 ,8528 ,8527 ,8527 ,8526 ,8524 ,8527 ,8526 ,8526 ,8528 ,8526 ,8523 ,8525 ,8527 ,8529 ,8523 ,8525 ,8524 ,8528 ,8525 ,8523 ,8523 ,8527 ,8526 ,8522 ,8525 ,8523 ,8525 ,8524 ,8524 ,8527 ,8525 ,8525 ,8525 ,8523 ,8527 ,8527 ,8527 ,8524 ,8523 ,8526 ,8523 ,8525 ,8525 ,8528 ,8525 ,8528 ,8527 ,8525 ,8523 ,8527 ,8525 ,8522 ,8527 ,8526 ,8523 ,8528 ,8525 ,8524 ,8527 ,8525 ,8526 ,8523 ,8522 ,8523 ,8526 ,8522 ,8526 ,8528 ,8524 ,8522 ,8525 ,8524 ,8522 ,8525 ,8523 ,8522 ,8524 ,8522 ,8522 ,8526 ,8523 ,8523 ,8526 ,8520 ,8525 ,8526 ,8523 ,8520 ,8520 ,8521 ,8523 ,8522 ,8520 ,8524 ,8523 ,8525 ,8520 ,8522 ,8521 ,8524 ,8520 ,8522 ,8521 ,8523 ,8521 ,8523 ,8521 ,8519 ,8523 ,8520 ,8522 ,8524 ,8524 ,8520 ,8522 ,8523 ,8520 ,8521 ,8521 ,8521 ,8524 ,8522 ,8522 ,8520 ,8523 ,8524 ,8521 ,8522 ,8521 ,8521 ,8522 ,8523 ,8522 ,8519 ,8523 ,8519 ,8522 ,8521 ,8522 ,8518 ,8519 ,8519 ,8521 ,8523 ,8518 ,8521 ,8522 ,8518 ,8520 ,8520 ,8521 ,8520 ,8522 ,8519 ,8521 ,8523 ,8519 ,8518 ,8519 ,8521 ,8520 ,8517 ,8518 ,8520 ,8521 ,8520 ,8522 ,8520 ,8518 ,8518 ,8520 ,8521 ,8518 ,8518 ,8522 ,8518 ,8518 ,8517 ,8520 ,8520 ,8521 ,8520 ,8521 ,8522 ,8517 ,8518 ,8519 ,8520 ,8521 ,8519 ,8518 ,8517 ,8519 ,8518 ,8515 ,8519 ,8518 ,8516 ,8516 ,8518 ,8517 ,8518 ,8518 ,8517 ,8518 ,8519 ,8519 ,8515 ,8515 ,8517 ,8520 ,8517 ,8517 ,8517 ,8516 ,8518 ,8521 ,8517 ,8517 ,8521 ,8521 ,8521 ,8516 ,8517 ,8515 ,8515 ,8519 ,8517 ,8519 ,8521 ,8517 ,8516 ,8516 ,8514 ,8518 ,8517 ,8515 ,8519 ,8517 ,8517 ,8514 ,8517 ,8517 ,8513 ,8515 ,8520 ,8513 ,8514 ,8518 ,8517 ,8518 ,8515 ,8515 ,8516 ,8517 ,8513 ,8515 ,8517 ,8517 ,8517 ,8514 ,8512 ,8517 ,8518 ,8512 ,8514 ,8518 ,8513 ,8519 ,8515 ,8517 ,8513 ,8518 ,8517 ,8516 ,8516 ,8515 ,8514 ,8513 ,8513 ,8512 ,8514 ,8513 ,8512 ,8516 ,8512 ,8511 ,8517 ,8517 ,8516 ,8514 ,8516 ,8513 ,8515 ,8517 ,8515 ,8513 ,8514 ,8511 ,8513 ,8515 ,8515 ,8515 ,8512 ,8511 ,8510 ,8512 ,8512 ,8514 ,8515 ,8516 ,8511 ,8515 ,8515 ,8513 ,8517 ,8516 ,8512 ,8513 ,8518 ,8512 ,8516 ,8515 ,8512 ,8511 ,8514 ,8516 ,8514 ,8515 ,8511 ,8515 ,8511 ,8511 ,8513 ,8511 ,8512 ,8514 ,8511 ,8514 ,8512 ,8514 ,8513 ,8511 ,8512 ,8514 ,8512 ,8513 ,8513 ,8514 ,8514 ,8514 ,8515 ,8512 ,8510 ,8514 ,8511 ,8510 ,8512 ,8510 ,8513 ,8514 ,8513 ,8513 ,8512 ,8514 ,8508 ,8513 ,8510 ,8509 ,8514 ,8512 ,8512 ,8510 ,8514 ,8511 ,8512 ,8515 ,8514 ,8512 ,8510 ,8511 ,8513 ,8510 ,8510 ,8509 ,8513 ,8509 ,8510 ,8514 ,8514 ,8509 ,8509 ,8513 ,8513 ,8513 ,8511 ,8511 ,8513 ,8512 ,8509 ,8508 ,8510 ,8512 ,8511 ,8511 ,8509 ,8511 ,8512 ,8510 ,8510 ,8513 ,8510 ,8508 ,8508 ,8510 ,8513 ,8513 ,8509 ,8511 ,8514 ,8513 ,8511 ,8513 ,8512 ,8512 ,8514 ,8511 ,8508 ,8511 ,8510 ,8507 ,8508 ,8510 ,8512 ,8512 ,8513 ,8509 ,8508 ,8510 ,8508 ,8508 ,8513 ,8513 ,8511 ,8509 ,8508 ,8506 ,8509 ,8511 ,8510 ,8512 ,8506 ,8507 ,8509 ,8510 ,8510 ,8510 ,8510 ,8510 ,8510 ,8510 ,8512 ,8509 ,8513 ,8510 ,8508 ,8511 ,8508 ,8506 ,8507 ,8508 ,8510 ,8510 ,8509 ,8505 ,8509 ,8508 ,8510 ,8509 ,8509 ,8506 ,8506 ,8507 ,8507 ,8506 ,8506 ,8507 ,8510 ,8508 ,8509 ,8507 ,8510 ,8512 ,8508 ,8508 ,8507 ,8509 ,8508 ,8508 ,8507 ,8507 ,8509 ,8506 ,8505 ,8510 ,8506 ,8505 ,8510 ,8506 ,8505 ,8507 ,8508 ,8505 ,8507 ,8507 ,8507 ,8509 ,8505 ,8505 ,8505 ,8506 ,8503 ,8504 ,8507 ,8504 ,8506 ,8507 ,8506 ,8505 ,8508 ,8506 ,8507 ,8507 ,8507 ,8505 ,8508 ,8506 ,8506 ,8505 ,8506 ,8503 ,8508 ,8509 ,8504 ,8506 ,8509 ,8506 ,8508 ,8508 ,8506 ,8506 ,8505 ,8507 ,8508 ,8508 ,8504 ,8507 ,8505 ,8506 ,8506 ,8505 ,8507 ,8505 ,8507 ,8504 ,8504 ,8503 ,8504 ,8505 ,8507 ,8506 ,8503 ,8505 ,8507 ,8507 ,8507 ,8506 ,8504 ,8503 ,8508 ,8508 ,8503 ,8506 ,8507 ,8507 ,8508 ,8508 ,8504 ,8506 ,8503 ,8506 ,8506 ,8503 ,8502 ,8505 ,8505 ,8507 ,8505 ,8507 ,8502 ,8504 ,8504 ,8507 ,8509 ,8506 ,8507 ,8507 ,8506 ,8503 ,8503 ,8504 ,8503 ,8503 ,8505 ,8503 ,8505 ,8504 ,8505 ,8505 ,8507 ,8506 ,8504 ,8505 ,8506 ,8505 ,8504 ,8507 ,8504 ,8506 ,8506 ,8505 ,8504 ,8506 ,8505 ,8502 ,8507 ,8505 ,8503 ,8503 ,8502 ,8506 ,8506 ,8505 ,8505 ,8504 ,8504 ,8503 ,8503 ,8502 ,8503 ,8503 ,8504 ,8503 ,8504 ,8502 ,8504 ,8505 ,8504 ,8502 ,8503 ,8503 ,8503 ,8503 ,8504 ,8501 ,8501 ,8501 ,8503 ,8503 ,8503 ,8503 ,8500 ,8503 ,8502 ,8503 ,8507 ,8506 ,8501 ,8502 ,8503 ,8504 ,8503 ,8504 ,8504 ,8504 ,8503 ,8502 ,8501 ,8501 ,8503 ,8503 ,8503 ,8502 ,8501 ,8505 ,8504 ,8504 ,8501 ,8503 ,8501 ,8501 ,8502 ,8503 ,8500 ,8501 ,8502 ,8502 ,8502 ,8503 ,8500 ,8502 ,8503 ,8499 ,8500 ,8500 ,8501 ,8500 ,8502 ,8501 ,8502 ,8504 ,8501 ,8502 ,8503 ,8500 ,8500 ,8500 ,8501 ,8502 ,8501 ,8502 ,8501 ,8502 ,8501 ,8500 ,8501 ,8501 ,8500 ,8503 ,8503 ,8500 ,8500 ,8499 ,8500 ,8500 ,8501 ,8500 ,8500 ,8502 ,8502 ,8498 ,8503 ,8501 ,8501 ,8502 ,8502 ,8500 ,8503 ,8500 ,8500 ,8501 ,8503 ,8502 ,8502 ,8501 ,8502 ,8499 ,8500 ,8498 ,8500 ,8498 ,8502 ,8502 ,8503 ,8500 ,8501 ,8497 ,8500 ,8498 ,8498 ,8501 ,8501 ,8501 ,8503 ,8501 ,8497 ,8502 ,8502 ,8501 ,8499 ,8498 ,8498 ,8499 ,8499 ,8498 ,8500 ,8502 ,8501 ,8499 ,8498 ,8499 ,8499 ,8498 ,8501 ,8499 ,8498 ,8499 ,8500 ,8497 ,8499 ,8500 ,8498 ,8496 ,8498 ,8499 ,8498 ,8498 ,8501 ,8497 ,8500 ,8498 ,8500 ,8498 ,8501 ,8501 ,8499 ,8498 ,8497 ,8499 ,8498 ,8497 ,8500 ,8501 ,8500 ,8498 ,8502 ,8501 ,8499 ,8501 ,8501 ,8496 ,8500 ,8501 ,8496 ,8498 ,8499 ,8497 ,8497 ,8498 ,8499 ,8501 ,8500 ,8498 ,8500 ,8498 ,8498 ,8496 ,8498 ,8499 ,8494 ,8501 ,8500 ,8501 ,8496 ,8499 ,8497 ,8499 ,8501 ,8498 ,8498 ,8497 ,8495 ,8495 ,8499 ,8497 ,8498 ,8498 ,8498 ,8498 ,8498 ,8498 ,8494 ,8497 ,8499 ,8497 ,8496 ,8497 ,8496 ,8497 ,8500 ,8498 ,8495 ,8500 ,8498 ,8497 ,8498 ,8497 ,8498 ,8497 ,8494 ,8496 ,8496 ,8495 ,8499 ,8497 ,8497 ,8495 ,8496 ,8498 ,8498 ,8496 ,8498 ,8499 ,8498 ,8496 ,8497 ,8499 ,8494 ,8498 ,8496 ,8496 ,8497 ,8495 ,8494 ,8498 ,8495 ,8499 ,8496 ,8498 ,8497 ,8498 ,8498 ,8495 ,8498 ,8497 ,8497 ,8497 ,8499 ,8495 ,8495 ,8496 ,8495 ,8497 ,8498 ,8495 ,8496 ,8495 ,8498 ,8495 ,8496 ,8497 ,8496 ,8495 ,8495 ,8498 ,8495 ,8498 ,8498 ,8497 ,8497 ,8495 ,8494 ,8496 ,8496 ,8493 ,8494 ,8494 ,8493 ,8494 ,8497 ,8495 ,8496 ,8496 ,8496 ,8494 ,8497 ,8496 ,8497 ,8495 ,8498 ,8494 ,8494 ,8497 ,8495 ,8493 ,8494 ,8495 ,8496 ,8495 ,8495 ,8492 ,8492 ,8497 ,8496 ,8494 ,8496 ,8493 ,8493 ,8492 ,8496 ,8495 ,8497 ,8496 ,8495 ,8492 ,8493 ,8493 ,8493 ,8495 ,8496 ,8494 ,8495 ,8493 ,8497 ,8494 ,8492 ,8492 ,8495 ,8496 ,8496 ,8495 ,8496 ,8494 ,8493 ,8494 ,8493 ,8493 ,8496 ,8493 ,8494 ,8492 ,8495 ,8495 ,8492 ,8494 ,8493 ,8492 ,8495 ,8495 ,8494 ,8494 ,8493 ,8492 ,8494 ,8493 ,8494 ,8491 ,8496 ,8494 ,8492 ,8494 ,8493 ,8493 ,8495 ,8496 ,8493 ,8494 ,8495 ,8494 ,8490 ,8495 ,8493 ,8491 ,8493 ,8493 ,8491 ,8490 ,8494 ,8492 ,8492 ,8495 ,8492 ,8490 ,8495 ,8494 ,8493 ,8495 ,8493 ,8493 ,8495 ,8494 ,8494 ,8494 ,8493 ,8493 ,8490 ,8490 ,8493 ,8491 ,8493 ,8493 ,8493 ,8493 ,8491 ,8492 ,8491 ,8491 ,8493 ,8493 ,8493 ,8492 ,8492 ,8493 ,8492 ,8492 ,8490 ,8491 ,8495 ,8492 ,8492 ,8494 ,8492 ,8492 ,8494 ,8491 ,8490 ,8495 ,8492 ,8492 ,8494 ,8494 ,8493 ,8493 ,8492 ,8490 ,8490 ,8490 ,8492 ,8491 ,8490 ,8490 ,8492 ,8492 ,8492 ,8490 ,8489 ,8491 ,8491 ,8490 ,8492 ,8491 ,8493 ,8492 ,8489 ,8491 ,8493 ,8493 ,8493 ,8491 ,8492 ,8493 ,8489 ,8489 ,8489 ,8490 ,8488 ,8491 ,8492 ,8491 ,8490 ,8491 ,8489 ,8490 ,8492 ,8490 ,8491 ,8491 ,8490 ,8488 ,8491 ,8488 ,8489 ,8487 ,8490 ,8490 ,8489 ,8490 ,8491 ,8494 ,8489 ,8489 ,8493 ,8491 ,8493 ,8490 ,8493 ,8491 ,8490 ,8490 ,8492 ,8491 ,8493 ,8492 ,8492 ,8493 ,8491 ,8491 ,8490 ,8490 ,8491 ,8492 ,8492 ,8492 ,8490 ,8488 ,8491 ,8490 ,8489 ,8490 ,8492 ,8492 ,8488 ,8490 ,8489 ,8490 ,8491 ,8491 ,8488 ,8488 ,8489 ,8491 ,8489 ,8490 ,8492 ,8489 ,8493 ,8491 ,8489 ,8490 ,8493 ,8490 ,8490 ,8492 ,8494 ,8493 ,8490 ,8490 ,8493 ,8491 ,8491 ,8491 ,8492 ,8489 ,8492 ,8491 ,8491 ,8492 ,8491 ,8491 ,8487 ,8488 ,8490 ,8492 ,8488 ,8494 ,8491 ,8491 ,8491 ,8493 ,8491 ,8492 ,8492 ,8491 ,8489 ,8491 ,8490 ,8489 ,8488 ,8489 ,8488 ,8487 ,8487 ,8490 ,8489 ,8488 ,8487 ,8488 ,8488 ,8488 ,8490 ,8489 ,8487 ,8490 ,8491 ,8488 ,8489 ,8488 ,8489 ,8488 ,8490 ,8490 ,8489 ,8488 ,8488 ,8491 ,8487 ,8487 ,8489 ,8487 ,8488 ,8486 ,8489 ,8488 ,8488 ,8487 ,8489 ,8487 ,8489 ,8488 ,8487 ,8488 ,8487 ,8488 ,8490 ,8491 ,8487 ,8488 ,8489 ,8487 ,8489 ,8489 ,8489 ,8490 ,8488 ,8489 ,8490 ,8488 ,8487 ,8490 ,8489 ,8487 ,8486 ,8487 ,8489 ,8486 ,8487 ,8487 ,8486 ,8488 ,8486 ,8485 ,8486 ,8486 ,8487 ,8488 ,8489 ,8488 ,8487 ,8487 ,8489 ,8486 ,8487 ,8486 ,8487 ,8483 ,8487 ,8485 ,8486 ,8487 ,8486 ,8488 ,8486 ,8489 ,8487 ,8485 ,8485 ,8489 ,8487 ,8484 ,8486 ,8487 ,8484 ,8488 ,8486 ,8486 ,8489 ,8489 ,8488 ,8484 ,8486 ,8486 ,8485 ,8487 ,8488 ,8485 ,8485 ,8488 ,8488 ,8487 ,8486 ,8487 ,8486 ,8486 ,8485 ,8486 ,8487 ,8486 ,8487 ,8484 ,8487 ,8485 ,8487 ,8487 ,8485 ,8484 ,8485 ,8484 ,8486 ,8484 ,8486 ,8483 ,8484 ,8486 ,8485 ,8485 ,8485 ,8486 ,8484 ,8486 ,8486 ,8484 ,8485 ,8487 ,8484 ,8486 ,8485 ,8484 ,8486 ,8484 ,8485 ,8485 ,8486 ,8484 ,8485 ,8485 ,8485 ,8486 ,8486 ,8483 ,8482 ,8486 ,8483 ,8486 ,8485 ,8484 ,8485 ,8485 ,8485 ,8485 ,8485 ,8484 ,8485 ,8483 ,8482 ,8482 ,8484 ,8484 ,8485 ,8485 ,8485 ,8484 ,8482 ,8483 ,8483 ,8481 ,8484 ,8485 ,8482 ,8483 ,8482 ,8485 ,8484 ,8484 ,8483 ,8482 ,8484 ,8484 ,8484 ,8481 ,8484 ,8481 ,8483 ,8484 ,8482 ,8483 ,8483 ,8482 ,8484 ,8481 ,8485 ,8482 ,8484 ,8484 ,8482 ,8484 ,8484 ,8483 ,8483 ,8482 ,8485 ,8484 ,8482 ,8484 ,8483 ,8483 ,8484 ,8484 ,8484 ,8484 ,8485 ,8482 ,8482 ,8483 ,8482 ,8484 ,8483 ,8485 ,8482 ,8482 ,8482 ,8481 ,8484 ,8482 ,8484 ,8483 ,8483 ,8482 ,8484 ,8484 ,8484 ,8482 ,8484 ,8479 ,8483 ,8481 ,8482 ,8482 ,8482 ,8483 ,8482 ,8481 ,8482 ,8481 ,8480 ,8480 ,8483 ,8480 ,8482 ,8480 ,8481 ,8482 ,8482 ,8482 ,8481 ,8480 ,8479 ,8479 ,8479 ,8481 ,8480 ,8480 ,8482 ,8483 ,8481 ,8479 ,8479 ,8482 ,8482 ,8481 ,8480 ,8482 ,8479 ,8479 ,8480 ,8483 ,8482 ,8480 ,8482 ,8484 ,8481 ,8482 ,8483 ,8482 ,8482 ,8480 ,8480 ,8479 ,8481 ,8482 ,8481 ,8481 ,8482 ,8482 ,8480 ,8480 ,8479 ,8481 ,8483 ,8481 ,8485 ,8480 ,8480 ,8480 ,8480 ,8480 ,8480 ,8483 ,8482 ,8482 ,8479 ,8482 ,8481 ,8480 ,8481 ,8481 ,8479 ,8479 ,8481 ,8483 ,8481 ,8480 ,8482 ,8479 ,8481 ,8482 ,8481 ,8479 ,8480 ,8481 ,8481 ,8478 ,8481 ,8480 ,8480 ,8480 ,8479 ,8481 ,8481 ,8478 ,8481 ,8477 ,8480 ,8477 ,8479 ,8479 ,8478 ,8479 ,8481 ,8479 ,8481 ,8478 ,8480 ,8480 ,8480 ,8480 ,8479 ,8477 ,8480 ,8478 ,8479 ,8476 ,8478 ,8479 ,8478 ,8478 ,8479 ,8476 ,8478 ,8480 ,8479 ,8477 ,8479 ,8481 ,8478 ,8478 ,8477 ,8480 ,8477 ,8477 ,8480 ,8478 ,8478 ,8477 ,8479 ,8479 ,8478 ,8476 ,8478 ,8478 ,8478 ,8478 ,8478 ,8476 ,8479 ,8478 ,8479 ,8476 ,8476 ,8478 ,8478 ,8475 ,8478 ,8480 ,8478 ,8479 ,8479 ,8479 ,8479 ,8478 ,8479 ,8479 ,8476 ,8478 ,8480 ,8477 ,8478 ,8477 ,8479 ,8478 ,8476 ,8479 ,8477 ,8475 ,8478 ,8476 ,8479 ,8478 ,8477 ,8480 ,8477 ,8479 ,8478 ,8477 ,8478 ,8478 ,8476 ,8479 ,8478 ,8478 ,8477 ,8478 ,8479 ,8477 ,8480 ,8478 ,8480 ,8477 ,8479 ,8479 ,8476 ,8478 ,8480 ,8477 ,8479 ,8480 ,8480 ,8478 ,8479 ,8478 ,8477 ,8478 ,8478 ,8479 ,8478 ,8477 ,8477 ,8479 ,8478 ,8480 ,8478 ,8479 ,8477 ,8477 ,8478 ,8481 ,8479 ,8479 ,8479 ,8480 ,8480 ,8479 ,8476 ,8480 ,8479 ,8476 ,8478 ,8477 ,8479 ,8477 ,8480 ,8480 ,8477 ,8478 ,8477 ,8476 ,8478 ,8476 ,8479 ,8481 ,8477 ,8480 ,8481 ,8479 ,8476 ,8478 ,8479 ,8477 ,8477 ,8478 ,8477 ,8477 ,8475 ,8479 ,8476 ,8476 ,8475 ,8474 ,8478 ,8475 ,8476 ,8476 ,8479 ,8476 ,8477 ,8478 ,8476 ,8479 ,8476 ,8478 ,8476 ,8475 ,8477 ,8477 ,8475 ,8476 ,8479 ,8476 ,8478 ,8474 ,8478 ,8476 ,8476 ,8476 ,8476 ,8478 ,8476 ,8477 ,8476 ,8478 ,8475 ,8476 ,8476 ,8476 ,8477 ,8475 ,8477 ,8475 ,8474 ,8474 ,8477 ,8475 ,8475 ,8477 ,8475 ,8477 ,8474 ,8476 ,8475 ,8478 ,8475 ,8477 ,8475 ,8476 ,8477 ,8477 ,8477 ,8476 ,8475 ,8476 ,8476 ,8475 ,8475 ,8476 ,8478 ,8473 ,8475 ,8476 ,8475 ,8477 ,8476 ,8478 ,8476 ,8475 ,8476 ,8476 ,8473 ,8472 ,8475 ,8476 ,8474 ,8475 ,8475 ,8474 ,8476 ,8475 ,8475 ,8478 ,8473 ,8472 ,8473};
int peak_counter = 0;

int peak_buffer[peak_candidate];
int previous_buf = peak_buffer[0];
int peak_count = 0;
int position[10] = {0};
int position_average = 0;
int position_counter = 1;
int sample_size=0;
int ref_position[2];

int finding_baseline();
void process_data();
void find_peak();
void store_history();

void EEPROM_setup(){
  if(!EEPROM.begin(20)){
      printf("failed to mount EEPROM");
  }
  else{
    printf("EEPROM begin\n");
    }
}

void check_sample(){
  for(int i = 0; i < sizeof(Sensor_arr)/sizeof(Sensor_arr[0])-10; i ++){
    if(Sensor_arr[i]!=0){
      sample_size +=1;
    }
    else{
      break;
    }
  }
  // printf("Sample size: %d\n",sample_size);
}
void process_data(){
  for(int i = 0; i < sample_size-50; i ++){
      int sum = 0;
      double mean;
      for( int j = 0; j <50 ; j++){
        sum = sum + Sensor_arr[i+j];
      } 
      mean = sum/50.00;
      delay(1);
      printf("%.2f\n",mean);
     Sensor_arr[i] = mean;
  }
}

double get_std(double mean){
  float sum_1 = 0;
  double sample = 1;
  double std = 0;
  printf("imported mean: %.2f\n",mean);
  for(int i =0; i < sample_size-10;i++){
    if (Sensor_arr[i] != 0){
      sum_1 += sq((double)(Sensor_arr[i]-mean));

      std = sqrt(sum_1/sample);
      // printf("sample: %d, sum: %.2f, std:%.2f\n",Sensor_arr[i], sum_1, std);
      
      sample += 1;
    }
  }
  std = sqrt(sum_1/sample);
  printf("std:%.2f,sample size; %f\n",std,sample);
  return std;
}

double get_mean(){
  long sum=0;
  double sample =1;
  double mean=0;
  for(int i =0; i < sample_size-10;i++){
    if (Sensor_arr[i] != 0){
      sum += Sensor_arr[i];
      // printf("sample: %d, sum: %.2d\n",Sensor_arr[i], sum);
      sample +=1;
    }
  }
  mean = sum/sample;
  printf("mean:%.2f,sample size:%f\n",mean,sample);
  return mean;
}

// void find_peak(){
//   printf("average data");
//   double mean = get_mean();
//   double std = get_std(mean);
//   double threshold = mean + 1* std;
//   printf("threshold: %.2f\n", threshold);  
//   bool isIncreasingslope = false;
//   bool isSlowinggradient = false;
//   bool isPlateau = false;
//   int prevSlope;
//   int Slope;
//   int nextSlope;    
//   int checkPlateau = 0;
//   int checkNotplateau = 0;
//   int checkSlope = 0;
//   int checkNotSlope = 0;
//   for (int q = 9; q < sample_size-50 ; ){
//     if(isIncreasingslope == false){
//       prevSlope = Sensor_arr[q] - Sensor_arr[q- 9];
//       Slope = Sensor_arr[q + 9]-Sensor_arr[q];
//       nextSlope = Sensor_arr[q + 19] -Sensor_arr[q+9];
//       // printf("Position: %d: prevSlope: %d, Slope: %d, nextSlope: %d\n", q, prevSlope,Slope, nextSlope);
//       if( Slope -prevSlope >-2 && nextSlope- Slope> -2){
//         checkSlope += 1;
//       }
//       else{
//         checkNotSlope +=1;
//       }
//       if(checkSlope > 5){
//         isIncreasingslope = true;
//         printf("Position: %d , potential peak..checking\n", q);
//         checkSlope = 0;
//         checkNotSlope = 0;
//       }
//       if(checkNotSlope > 2){
//         isIncreasingslope = false;
//         checkSlope = 0;
//         checkNotSlope = 0;
//       }
//       q+=5;
//     }

//     if(isIncreasingslope == true && isSlowinggradient == false){
//       prevSlope = Sensor_arr[q] - Sensor_arr[q-5];
//       Slope = Sensor_arr[q + 4]-Sensor_arr[q];
//       nextSlope = Sensor_arr[q + 10] -Sensor_arr[q+5];
//       printf("Position: %d: prevSlope: %d, Slope: %d, nextSlope: %d\n", q, prevSlope,Slope, nextSlope);
//       if(prevSlope > Slope && Slope > nextSlope && nextSlope >=0 ){
//         isSlowinggradient = true;
//         printf("Position: %d, gradient slowing down\n", q);
//       }
//       q+=1;
//     }
 
//     if(isIncreasingslope == true && isSlowinggradient == true){
//       if(abs(Sensor_arr[q]- Sensor_arr[q-10]) <3){
//         checkPlateau +=1;
//         printf("plateauing..waiting\n");
//       }
//       else{
//         checkNotplateau +=1;
//       }
//       if(checkPlateau >10){
//         position[peak_count] = q;
//         peak_count +=1;
//         printf("Peak %d, position : %d\n", peak_count, q);
//         isIncreasingslope = false;
//         isSlowinggradient = false;
//         checkNotplateau =0;
//         checkPlateau =0;
//       }
//       if(checkNotplateau > 5){
//         printf("not plateau.. reset\n");
//         isIncreasingslope = false;
//         isSlowinggradient = false;
//         checkNotplateau =0;
//         checkPlateau =0;
//       }
//       q+=1;
//     }
//     // if (Sensor_arr[q] != 0){
//     //   if(Sensor_arr[q]>(int)(threshold)){
//     //     peak_buffer[peak_counter] = q;
//     //     peak_counter += 1;
//     //   }
//     //   printf("sample%d: %.2d,threshold: %.2f\n",q, Sensor_arr[q],threshold);
//     // }
//   }

//   previous_buf = peak_buffer[0];
//   printf("Finding peak...");
//   // for (int j = 0; j < peak_candidate; j++){
//   //   if(peak_buffer[j] != 0){
//   //     printf("Grabbed point %d : %d\n",j+1,peak_buffer[j]);
//   //     if(peak_buffer[j]-previous_buf >100){
//   //       position_average = position_average/position_counter;
//   //       position[peak_count-1]= position_average;
//   //       printf("Peak_count: %d\n",peak_count);
//   //       printf("Peak position: %d\n", position[peak_count-1]);
        
//   //       peak_count +=1;

//   //       position_average = 0;
//   //       position_counter = 1;
//   //     }

//   //     position_average += peak_buffer[j];
//   //     // printf("position sum: %d\n",position_average);
//   //     position_counter += 1;
//   //     // printf("position counte: %d\n",position_counter);
//   //     previous_buf = peak_buffer[j];
//   //   }
//   // }
//   // if(position_counter !=1){
//   //   position_average = position_average/position_counter;
//   //   position[peak_count-1]= position_average;
//   // }
//   printf("Peak Total: %d\n", peak_count);
  
//   for (int i =0; i< peak_count+1; i++){
//     printf("Peak position: %d\n", position[i]);
//   }

//   // for (int i = 0; i<100; i++){
//   //   peak_buffer[i] =0;
//   // }
// }

void update_parameters(int unit){
  EEPROM.begin(20);
  printf("EEPROM begin\n");
    int past =0; 
    byte address = 0;
    EEPROM.get(0,past);
    printf("EEPROM write value : %d\n", past);

    ref_position[0] = position[0] * unit;
    ref_position[1] = position[1] *unit;
    printf("updated: %d\n",ref_position[0]);
   
    EEPROM.put(address, ref_position[0]);  
    delay(100);  
    // printf("EEPROM address: %d, value: %d\n", address,position[0]);
    address += sizeof(int);
    EEPROM.put(address, ref_position[1]);  
    delay(100); 
    // printf("EEPROM address: %d, value: %d\n", address, position[1]);

    EEPROM.commit();
    address = 0;
    EEPROM.get(address,past);
    delay(500);
    printf("EEPROM write address: %d, value : %d\n",address, past);
    address += sizeof(int);
    EEPROM.get(address,past);
    delay(500);
    printf("EEPROM write address: %d, value : %d\n",address, past);
    EEPROM.end();
    delay(500);
}

void  calibration() { //put your main code here, to run repeatedly:
  PID_control();
  long previous = millis(); 
  long previous_2 =0 ;
  int entry_counter = 0;

  int j = 0;
  int time = 0;
  int num = 3;
  tft.setTextColor(TFT_WHITE, TFT_NEIGHBOUR_GREEN);
  bool fillscreen = true;
  bool istenth=true;
  for(int i =0; i<store_size; i++){
    Sensor_arr[i]=0;
  }
  long starttime = millis();
  while(millis() - previous < sampletime){
     PID_control();
    if(millis() - time > waittime && num >0){
      tft.drawString("Sample in ",110,120,4);
      tft.drawFloat(float(num),0,180,120,4);
      num--;
      time = millis();
    }
    //work simultaneously?
    if(millis()-time >1000 && num == 0 && fillscreen == true){
    tft.fillRect(0,100,240,40,TFT_NEIGHBOUR_GREEN);
    fillscreen = false;
    }
    // if(num == 0 && counter == 0 && millis() - time > waittime){
    // if(counter == 0 && millis() - time > waittime){
    //   tft.fillRect(0,100,240,40,TFT_NEIGHBOUR_GREEN);
    //   counter++;
    // }
    // printf("%d\n",time);
    // printf("%d\n",num);
    if(millis() - time > waittime){
      tft.setTextDatum(4);
      tft.drawString("Remain ",100,120,4);
      // if((sampletime-(millis() - previous))/waittime < 10 && istenth ==true){
      //   tft.fillRect(150,100,40,40,TFT_NEIGHBOUR_GREEN);
      //   istenth = false;
      // }
      tft.fillRect(155,100,65,40,TFT_NEIGHBOUR_GREEN);
      tft.drawFloat(float((sampletime-(millis() - previous))/waittime),0,170,120,4);
      time= millis();
    }

    tft.pushImage(90, 150, LoadingWidth  ,LoadingHeight, Loading[j]);
    delay(10);
    j++;
    if(j>10){
      j = 0;
    }
    // tft.drawFloat(float((9000-millis())/1000),0,200,120,2);
    
    // printf("%d\n", millis());

    if (millis()-previous_2>10){
      // Serial.print("Start:");Serial.println(millis());  
      Sensor_arr[entry_counter] = ads.readADC_SingleEnded(0);
      Serial.println(Sensor_arr[entry_counter]);
      entry_counter += 1;
      //printf("Counter 1: %d\n", entry_counter);
      previous_2 = millis();
    }
  }
  
  // check_sample();

  // process_data();
  find_peak();//part to be corrected
  int exposedtime= millis()-starttime;
  Serial.print("exposed time:");Serial.println(exposedtime);
  int unittime = exposedtime/entry_counter;
  Serial.print("Unit time:");Serial.println(unittime);
  update_parameters(unittime);
  store_history();
}

void find_peak(){
  int max_1=0;
  int max_2=0;
  for(int i = 50; i <250; i++){
    // Serial.println(Sensor_arr[i]);
    if(Sensor_arr[i]>max_1){
      max_1=Sensor_arr[i];
      position[0] = i;
      // Serial.println(position[0]);
    }
  }
  Serial.println();
  for(int j = 800; j <1500; j++){
    // Serial.println(Sensor_arr[j]);
    if(Sensor_arr[j]> max_2){
      max_2=Sensor_arr[j];
      position[1] = j;
      // Serial.println(position[1]);
    }
  }

}
void store_history(){
  if(SPIFFS.exists("/Calibration")){
      SPIFFS.remove("/Calibration");
      delay(500);
      printf("removed file: %s\n","/Calibration");
    }
    printf("Storing into %s\n","/Calibration");

    File file = SPIFFS.open("/Calibration",FILE_WRITE);
    file.print(',');file.write('\n'); 
    for(int i =0; i <2000; i++){
      if(Sensor_arr[i] !=0){
        file.print(Sensor_arr[i]);file.print(',');file.write('\n'); 
      }
    }
    file.close();
}