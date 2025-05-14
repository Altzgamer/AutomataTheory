#!/usr/bin/env bash
# скрипт: dot2png.sh
# Описание: конвертирует все файлы dfa_*.dot, ast_*.dot, nfa_*.dot и dfa_minimize_*.dot в PNG в папку pngs

# Если нет совпадений — массивы будут пустые
shopt -s nullglob

# Папка для выходных PNG
output_dir="pngs"
mkdir -p "$output_dir"

# Список префиксов
prefixes=(dfa_ ast_ nfa_ dfa_minimize_)

for prefix in "${prefixes[@]}"; do
    for dotfile in "${prefix}"*.dot; do
        # Формируем имя выходного файла в папке pngs
        base="${dotfile%.dot}"
        pngfile="$output_dir/${base}.png"
        
        # Конвертация через Graphviz
        if dot -Tpng "$dotfile" -o "$pngfile"; then
            echo "OK: $dotfile → $pngfile"
        else
            echo "ERROR: не удалось конвертировать $dotfile" >&2
        fi
    done
done

