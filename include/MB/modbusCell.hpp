// Modbus for c++ <https://github.com/Mazurel/Modbus>
// Copyright (c) 2020 Mateusz Mazur aka Mazurel
// Licensed under: MIT License <http://opensource.org/licenses/MIT>

#pragma once

#include <cstdint>
#include <stdexcept>
#include <variant>

/**
 * Namespace that contains whole project
 */
namespace MB {
/**
 * @brief Class that represents single modbus cell, it is either coil(bool) or
 * reg(uint16_t).
 */
class ModbusCell {
private:
  std::variant<uint16_t, uint8_t, bool> _value;

public:
  /**
   * @brief Constructs cell with register type equal to 0.
   */
  constexpr ModbusCell() = default;

  /**
   * @brief Constructs cell with register value.
   * @param reg - Register value.
   */
  constexpr ModbusCell(uint16_t reg) : _value(reg){};

  /**
   * @brief Constructs cell with coil value.
   * @param coil - Coil value.
   */
  constexpr ModbusCell(bool coil) : _value(coil){};

  constexpr ModbusCell(uint8_t val) : _value(val){};

  ModbusCell(const ModbusCell &other) noexcept = default;
  ModbusCell &operator=(const ModbusCell &other) noexcept = default;
  ModbusCell &operator=(ModbusCell &&other) noexcept = default;

  /**
   * @brief Static wrapper for cell construction.
   * @param reg - Requested value.
   * @return New cell.
   */
  [[nodiscard]] static ModbusCell initReg(uint16_t reg) {
    return ModbusCell(reg);
  }

  /**
   * @brief Static wrapper for cell construction.
   * @param coil - Requested value.
   * @return New cell.
   */
  [[nodiscard]] static ModbusCell initCoil(bool coil) {
    return ModbusCell(coil);
  }

  [[nodiscard]] static ModbusCell initVal(uint8_t val) {
    return ModbusCell(val);
  }

  /**
   * @brief Checks if cell is coil.
   * @returns Boolean representing result.
   */
  [[nodiscard]] bool isCoil() const {
    return std::holds_alternative<bool>(_value);
  };

  /**
   * @brief Checks if cell is register.
   * @returns Boolean representing result.
   */
  [[nodiscard]] bool isReg() const {
    return std::holds_alternative<uint16_t>(_value);
  };

  [[nodiscard]] bool isVal() const {
    return std::holds_alternative<uint8_t>(_value);
  };

  /**
   * @brief Returns coil value, changes cell type if necessary.
   * @return Reference to coil value.
   */
  bool &coil() {
    if (isReg())
      _value = static_cast<bool>(std::get<uint16_t>(_value));
    else if (isVal())
      _value = static_cast<bool>(std::get<uint8_t>(_value));
    return std::get<bool>(_value);
  };

  /**
   * @brief Returns register value, changes cell type if necessary.
   * @return Reference to coil value.
   */
  uint16_t &reg() {
    if (isCoil())
      _value = static_cast<uint16_t>(std::get<bool>(_value));
    else if (isVal())
      _value = static_cast<uint16_t>(std::get<uint8_t>(_value));
    return std::get<uint16_t>(_value);
  };

  uint8_t &val() {
    if (isCoil())
      _value = static_cast<uint8_t>(std::get<bool>(_value));
    else if (isReg())
      _value = static_cast<uint8_t>(std::get<uint16_t>(_value));
    return std::get<uint8_t>(_value);
  };

  /**
   * @brief Returns coil value.
   * @return Reference to coil value.
   * @throws bad_variant_access - When cell is of different type as requested.
   */
  [[nodiscard]] const bool &coil() const { return std::get<bool>(_value); };

  /**
   * @brief Returns register value.
   * @return Reference to coil value.
   * @throws bad_variant_access - When cell is of different type as requested.
   */
  [[nodiscard]] const uint16_t &reg() const {
    return std::get<uint16_t>(_value);
  };

  [[nodiscard]] const uint8_t &val() const {
    return std::get<uint8_t>(_value);
  };

  /**
   * Creates string representation of the cell.
   * @return String representation.
   */
  [[nodiscard]] std::string toString() const noexcept {
    return isCoil() ? ((coil()) ? "true" : "false") : ((isReg()) ? std::to_string(reg()) : std::to_string(val()));
  }
};
} // namespace MB
