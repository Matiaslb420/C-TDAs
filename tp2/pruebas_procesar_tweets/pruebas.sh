#!/usr/bin/env bash

set -eu

RET="0"
PROG="$1"
TWEETS="/tweets.txt"  # En la imagen de Docker.
VALGRIND_RETRY=""

# Correr con diff y sin Valgrind.
echo "Ejecución de pruebas procesar_tweets, unitarias (tot. líneas=10):"
echo ""

for x in 0[12345]*.test; do
    b=${x%.test}
    printf "${b} "
    cat ${x}

    ($PROG $(< ${b}_args) <${b}_in >${b}_actual_out 2>${b}_actual_err && \
        diff -u ${b}_err ${b}_actual_err && \
        diff -u ${b}_out ${b}_actual_out && \
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

echo "Ejecución de pruebas medianas procesar_tweets (comparación aproximada):"
echo ""

for x in 0[6789]*.test; do
    b=${x%.test}
    printf "${b} "
    cat ${x}

    $PROG $(< ${b}_args) <${b}_in >${b}_actual_out || RET="$?"
    ./procesar_tweets_exacto --exacta ${b}_out --compare ${b}_actual_out $(< ${b}_args)

    if [[ $RET -gt 128 ]]; then
        VALGRIND_RETRY="$VALGRIND_RETRY ${b}"
    fi
    echo ""
done

for x in $VALGRIND_RETRY; do
    echo "Prueba ${b} de nuevo con Valgrind"
    valgrind --leak-check=full --track-origins=yes --error-exitcode=0 \
        --quiet --log-fd=3 $PROG $(<${b}_args) <${b}_in 3>&1 >/dev/null 2>&1
done

echo "Ejecución de pruebas de volumen procesar_tweets (comparación aproximada):"
echo ""

# Estas se hace a mano porque el archivo gigante de tweets no está en el
# repo (y tienen un archivo aparte con el número de líneas).
for x in 1*.test; do
    b=${x%.test}
    printf "${b} "
    cat ${x}

    head -n $(< ${b}_lines) $TWEETS | $PROG $(< ${b}_args) >${b}_actual_out || RET="$?"
    ./procesar_tweets_exacto --exacta ${b}_out --compare ${b}_actual_out $(< ${b}_args)

    echo ""
done

# Salir si hubo errores de diff.
if [[ $RET -ne 0 ]]; then
    exit $RET
fi

echo "Ejecución de pruebas con Valgrind:"
echo ""

for x in 0*.test; do
    b=${x%.test}
    printf "${b} "
    valgrind --leak-check=full --track-origins=yes --error-exitcode=2 \
        --quiet --log-fd=3 $PROG $(<${b}_args) <${b}_in 3>&1 >/dev/null 2>&1 && \
        echo "OK" || { RET=$?; echo "ERROR"; }
done

exit $RET
