/** @type {import('tailwindcss').Config} */
module.exports = {
  content: ["./index.html", "./**/*.html", "./**/**/*.html", "./**/**/**/*.html"],
  theme: {
    extend: {
      colors: {
        "light_gray": "#E1E5EA",
        "marine": "#0B1E3F",
        "vibrant_orange": "#FF6B00",
        "dark_gray": "#1C2738",
        "custom_green": "#00B894",
      }
    },
  },
  plugins: [],
}