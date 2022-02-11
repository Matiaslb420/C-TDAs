#!/usr/bin/env bash

set -eu

export TIME="Tardó: %Us - Consumió: %M KiB"
PROGRAMA="$1"
RET=0
VALGRIND_RETRY=""

# Correr con diff y sin Valgrind.
echo "Ejecución de pruebas unitarias procesar_usuarios:"
echo ""
for x in *.test; do

    b=${x%.test}
    printf "${b} "
    cat ${b}.test

   ($PROGRAMA $(< ${b}_in) > ${b}_actual_out 2> ${b}_actual_err && \
        diff -u ${b}_out ${b}_actual_out && \
        diff -u ${b}_err ${b}_actual_err && \
        echo "OK") || { RET=$?; echo "ERROR"; }

    if [[ $RET -gt 128 ]]; then
        # Si $RET es mayor que 128, el programa murió por una señal, en
        # particular por la señal de valor numérico $RET - 128; por ejemplo
        # 11 (segmentation fault) o 6 (abort).
        #
        # Para este tipo de fallos, corremos de nuevo bajo Valgrind la prueba,
        # para dar una posibilidad de saber, sin acceso a la prueba en sí, por
        # dónde viene el error.
        #
        # TODO: quizá usar una lista fija de señales si hay algunas para las que
        # no se debería correr de nuevo bajo Valgrind.
        VALGRIND_RETRY="$VALGRIND_RETRY ${b}"
    fi

    echo ""
done

for b in $VALGRIND_RETRY; do
    echo "Prueba ${b} de nuevo con Valgrind"
    valgrind --leak-check=full --track-origins=yes --error-exitcode=0 \
             --quiet --log-fd=3 $PROGRAMA $(< ${b}_in) 3>&1 >/dev/null 2>&1
done

# Salir si hubo errores de diff.
if [[ $RET -ne 0 ]]; then
    exit $RET
fi

echo "Ejecución de pruebas con Valgrind:"
# Correr con Valgrind y sin diff.
for x in *_in; do
    echo -n "${x%_in} "

    valgrind --leak-check=full --track-origins=yes --error-exitcode=2 \
        --quiet --log-fd=3 $PROGRAMA $(< $x) 3>&1 >/dev/null 2>&1 && \
        echo "OK" || { RET=$?; echo "ERROR"; }
done

exit $RET
