# Note d'Évaluation — ft_nm

Ce document explique les spécificités de tri de `ft_nm` et fournit les commandes pour vérifier la conformité du binaire lors de la soutenance.

---

## Important : Ordre de tri & Locales système (`LC_ALL`)

Le barème officiel 42 précise explicitement pour les fichiers objets (`.o`), bibliothèques dynamiques (`.so`) et universels :
> *"The symbol order can differ with the real nm."* / *"The symbol order can be arbitrary."*

### Pourquoi existe-t-il une variation d'ordre selon la machine ?
1. Le binaire système `nm` s'appuie sur la fonction `strcoll()` de la `libc`, qui dépend directement de la locale de la machine hôte (ex: `fr_FR.UTF-8` vs `C` / `POSIX`).
2. En locale `UTF-8` (comme sur Fedora), la collation traite la ponctuation (comme `_` ou `.`) différemment d'un tri ASCII standard (`strcmp`), ce qui décale légèrement l'ordre de symboles comme `_GLOBAL_OFFSET_TABLE_`.

---

## Commandes de Vérification pour l'Évaluateur

### 1. Test standard (Locale C / ASCII)
Pour tester le comportement de `ft_nm` dans l'environnement ASCII standard (identique au fonctionnement dans le conteneur Docker de dev) :

```bash
LC_ALL=C nm tests/out/basic64 > /tmp/nm.out
LC_ALL=C ./ft_nm tests/out/basic64 > /tmp/ft_nm.out
diff -u /tmp/nm.out /tmp/ft_nm.out