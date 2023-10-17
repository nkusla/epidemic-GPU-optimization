# Comparison of Epidemic Simulation Efficiency on a Central Processing Unit and a Graphics Processing Unit

Repository for scientific project at Petnica Science Center (2020)

## Abstract

In this paper, an epidemic simulation model using an agent based method, based on the SIR model is examined. The way in which the algorithm of such simulation can be modified is analyzed, so that in addition to execution on the processor, the simulation can also be executed on the graphics card. The simulation model had 15 parameters that could be changed and it was tested to see what types of epidemics it could simulate. The speed of performing such a simulation on the processor and graphics card was also examined. In addition, it was concluded that simulations (performed exclusively on a graphics card) that used atomic functions proved to be extremely better than those simulations that did not use those functions. Simulations with a small number of agents were executed a little faster on the processor, and simulations with an extremely large number of agents were executed much faster on the graphics card. Also, the results of the simulations were compared, so that we can see the difference between the predictions obtained from the simulations that were performed on two different computer components. Increasing the agents in the simulation reduces the difference in the prediction of the simulations.

## Paper

Full paper is written in Serbian and was presented at Petnica conference "A step into the science" in December 2020.

[Link to paper](./Paper/Zb2020-RAC-NK.pdf)

## Awards

Presented online at International scientific youth forum “Step Into The Future” in Russia, June 2021.
Awarded 3rd place in the field of Formal Sciences.

## References

[1] Aaby B. G., Perumalla K. S., Seal S. K. 2010. Efficient simulation of agent-based models on multi-GPU and multi-core clusters. U Proceedings of the 3rd international icst conference on simulation tools and techniques (ur. F. Perrone i G. Stea). Brussels: ICST (Institute for Computer Sciences, Social-Informatics and Telecommunications Engineering), str. 1-10.

[2] Feautrier P. 2011. Bernstein's conditions. U Encyclopedia of Parallel Computing (ur. A. Padua). Springer, str. 130-134.

[3] Jaffry S. W., Treur J. 2008. Agent-based and population-based simulation: A comparative case study for epidemics. U Proceedings of the 22nd European Conference on Modelling and Simulation. ECMS (The European Council for Modelling and Simulation), str. 123-130.

[5] Kermack W. O., McKendrick A. G. 1927. A contribution to the mathematical theory of epidemics. U Proceedings of the royal society of london. Series A. 115 (772): 700-21.

[6] Kovac T., Haber T., Van Reeth F., Hens N. 2018. Heterogeneous computing for epidemiological model fitting and simulation. BMC bioinformatics, 19 (1): 101-11.

[7] Matsumoto M., Nishimura T. 1998. Mersenne twister: a 623-dimensionally equidistributed uniform pseudo-random number generator. ACM Transactions on Modeling and Computer Simulation (TOMACS), 8 (1): 3-30.

[8] Shekh B., De Doncker E., Prieto D. 2015. Hybrid multi-threaded simulation of agent-based pandemic modeling using multiple GPUs. U 2015 IEEE International Conference on Bioinformatics and Biomedicine (BIBM). IEEE, str. 1478-85.

[9] Zou P., Lü Y. S., Wu L. D., Chen L. L., Yao Y. P. 2013 Epidemic simulation of a large-scale social contact network on GPU clusters. Simulation, 89 (10): 1154-72.